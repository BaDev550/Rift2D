#include "SceneRenderer2D.h"
#include <Runtime/Definitions/Profiler.h>
#include <Runtime/Memory/Allocator.h>

#include <Engine/Asset/AssetSubsystem.h>
#include <Engine/Platform/Window/WindowSubsystem.h>
#include <Engine/Graphics/GraphicsSubsystem.h>
#include <Engine/Asset/Shader/Shader.h>

#include <Engine/World/ECS/Scene.h>
#include <Engine/World/Components/TransformComponent.h>
#include <Engine/World/Components/PointLightComponent.h>

#include <Engine/World/Components/SpriteComponent.h>
#include <Engine/Render/Sprite/SpriteData.h>
#include <Engine/Render/Sprite/SpriteVertex.h>

#include <algorithm>
#include <imgui.h>

namespace Rift::Engine {
	Runtime::Result SceneRenderer2DSystem::OnCreate() {
		auto renderSystem = _engine->GetSubsystem<GraphicsSubsystem>();

		_forwardRenderPass = Runtime::Mem::Allocator::Construct<ForwardRenderPass>();
		_forwardRenderPass->SetEngine(_engine);
		_forwardRenderPass->OnInitialize();

		_occluderPass = Runtime::Mem::Allocator::Construct<OccluderPass>();
		_occluderPass->SetEngine(_engine);
		_occluderPass->OnInitialize();

		_lightPass = Runtime::Mem::Allocator::Construct<LightPass>(); // TODO(0x):RTTR
		_lightPass->SetEngine(_engine);
		_lightPass->OnInitialize();

		Runtime::RHI::BufferDesc desc{};
		desc.size = sizeof(SpriteVertices);
		desc.usage = Runtime::RHI::BufferUsage::Storage;
		desc.memoryUsage = Runtime::RHI::MemoryUsage::CPUToGPU;
		desc.allocationFlags = Runtime::RHI::AllocationFlagBits::CreateMapped | Runtime::RHI::AllocationFlagBits::HostAccessSequentialWrite;
		_spriteBuffer = renderSystem->GetContext()->CreateBuffer(desc);
		_spriteBuffer->Write(SpriteVertices);

		{
			for (auto& buffer : _frameDataBuffers) {
				Runtime::RHI::BufferDesc storageDesc{};
				storageDesc.size = sizeof(glm::mat4);
				storageDesc.usage = Runtime::RHI::BufferUsage::Storage;
				storageDesc.memoryUsage = Runtime::RHI::MemoryUsage::CPUToGPU;
				storageDesc.allocationFlags = Runtime::RHI::AllocationFlagBits::CreateMapped | Runtime::RHI::AllocationFlagBits::HostAccessSequentialWrite;
				buffer = renderSystem->GetContext()->CreateBuffer(storageDesc);
			}
			for (auto& buffer : _lightBuffers) {
				Runtime::RHI::BufferDesc lightDesc{};
				lightDesc.size = sizeof(LightData) * OccluderPass::MaxLights;
				lightDesc.usage = Runtime::RHI::BufferUsage::Storage | Runtime::RHI::BufferUsage::DeviceAddress;
				lightDesc.memoryUsage = Runtime::RHI::MemoryUsage::Auto;
				lightDesc.allocationFlags = Runtime::RHI::AllocationFlagBits::HostAccessRandom | Runtime::RHI::AllocationFlagBits::CreateMapped;
				buffer = renderSystem->GetContext()->CreateBuffer(lightDesc);
			}
		}

		return Runtime::Result();
	}

	void SceneRenderer2DSystem::OnUpdate(Scene& scene) {
		auto system = _engine->GetSubsystem<GraphicsSubsystem>();

		system->Submit([this, &scene](Runtime::RHI::CommandBuffer* cmd) {
			auto assetSystem = _engine->GetSubsystem<AssetSubsystem>();
			auto renderSystem = _engine->GetSubsystem<GraphicsSubsystem>();

			{
				BEGIN_PROFILE_SCOPE("ScenePass");

				RenderFrame frame{};
				frame.cmd = cmd;

				const bool offscreen = BeginOffscreen(renderSystem, cmd);
				if (offscreen) {
					frame.color = _viewportColor;
					frame.depth = _viewportDepth;
					frame.extent = glm::vec2(static_cast<f32>(_viewportWidth), static_cast<f32>(_viewportHeight));
				}
				else {
					frame.color = renderSystem->GetSwapchain()->GetColorAttachments()[renderSystem->GetImageIndex()];
					frame.depth = renderSystem->GetSwapchain()->GetDepthAttachment();
					frame.extent = renderSystem->GetSwapchain()->GetExtent();
				}
				frame.frameIndex = renderSystem->GetFrameIndex();
				frame.frameDataBufferIndex = _frameDataBuffers[frame.frameIndex]->GetBindlessIndex();

				const f32 aspect = frame.extent.x / frame.extent.y;
				const f32 halfH = 5.0f;
				glm::mat4 viewProj = glm::orthoRH_ZO(-halfH * aspect, halfH * aspect, -halfH, halfH, -1.0f, 1.0f);
				viewProj[1][1] *= -1.0f;
				_frameDataBuffers[frame.frameIndex]->Write(&viewProj);

				auto& spriteQueue = frame.queues.Get(RenderQueue::Sprite);
				scene.ForEach<TransformComponent, SpriteComponent>([&](EntityHandle entity, TransformComponent& transform, SpriteComponent& sprite) {
					if (!sprite.Visible) return;
					if (!sprite.TextureAssetHandle.IsValid()) return;

					Texture2D* asset = assetSystem->Get<Texture2D>(sprite.TextureAssetHandle);
					if (!asset || (asset->GetAssetState() != AssetState::Ready))
						return;

					transform.world =
						Runtime::Math::Mat4::Translation(transform.position) *
						Runtime::Math::Mat4::Rotate(transform.rotation) *
						Runtime::Math::Mat4::Scale(transform.scale);

					RenderItem item{};
					item.textureID = asset->_image->GetBindlessIndex();
					item.samplerID = asset->_image->GetSamplerIndex();
					item.world = transform.world;
					item.castShadow = sprite.CastShadow;
					item.firstVertex = 0;
					item.vertexCount = 6;
					item.instanceCount = 1;
					//item.drawOrder = BuildSpriteSort(sprite.zOrder, transform.position.y);
					item.drawOrder = sprite.zOrder;
					item.vertexBufferIndex = _spriteBuffer->GetBindlessIndex();
					spriteQueue.push_back(item);
					});
				std::stable_sort(spriteQueue.begin(), spriteQueue.end(), [](const RenderItem& a, const RenderItem& b) { return a.drawOrder < b.drawOrder; });

				std::vector<LightData> lights;
				scene.ForEach<TransformComponent, PointLightComponent>([&](EntityHandle entity, TransformComponent& transform, PointLightComponent& light) {
					if (lights.size() >= OccluderPass::MaxLights) return;

					LightData data{};
					data.lightPos = Runtime::Math::Vec2(transform.position.x, transform.position.y);
					data.lightRadius = light.radius;
					data.lightIntensity = light.intensity;
					data.lightColor = Runtime::Math::Vec4(light.color.x, light.color.y, light.color.z, 1.0f);
					lights.push_back(data);
					});

				u32 lightCount = static_cast<u32>(lights.size());
				u64 lightBDA = 0;
				if (lightCount > 0) {
					_lightBuffers[frame.frameIndex]->Write(lights.data(), sizeof(LightData) * lightCount);
					lightBDA = _lightBuffers[frame.frameIndex]->GetGPUAdress();
				}

				_occluderPass->SetLightData(lightBDA, lightCount);
				_lightPass->SetLightData(
					lightBDA,
					lightCount,
					_occluderPass->GetShadowMap1D(),
					static_cast<f32>(OccluderPass::LightMapResolution),
					static_cast<f32>(OccluderPass::MaxLights)
				);

				_occluderPass->Execute(frame);
				_forwardRenderPass->Execute(frame);
				_lightPass->Execute(frame);

				if (offscreen) cmd->TransitionImageLayout(_viewportColor, Runtime::RHI::ImageLayout::ShaderReadOnly);
			}
			});
	}

	void SceneRenderer2DSystem::OnDestroy() {
		auto renderSystem = _engine->GetSubsystem<GraphicsSubsystem>();
		renderSystem->GetContext()->WaitDeviceIdle();

		_forwardRenderPass->OnDestroy();
		_lightPass->OnDestroy();
		_occluderPass->OnDestroy();
		Mem::Allocator::Destroy(_lightPass);
		Mem::Allocator::Destroy(_occluderPass);
		Mem::Allocator::Destroy(_forwardRenderPass);

		RetireOffscreen();
		CollectRetired(renderSystem, true);

		for (auto& fdb : _frameDataBuffers) {
			Runtime::Mem::Allocator::Destroy(fdb);
		}

		for (auto& lb : _lightBuffers) {
			Runtime::Mem::Allocator::Destroy(lb);
		}

		Mem::Allocator::Destroy(_spriteBuffer);
	}

	bool SceneRenderer2DSystem::BeginOffscreen(GraphicsSubsystem* gfx, Runtime::RHI::CommandBuffer* cmd) {
		_offscreenFrame++;
		CollectRetired(gfx, false);

		if (!_viewportActive) return false;
		_viewportActive = false;

		const u32 w = std::max(_viewportRequestW, 1u);
		const u32 h = std::max(_viewportRequestH, 1u);

		if (!_viewportColor || w != _viewportWidth || h != _viewportHeight) {
			RetireOffscreen();
			CreateOffscreen(gfx, w, h);
		}

		cmd->TransitionImageLayout(_viewportColor, Runtime::RHI::ImageLayout::ColorAttachment);
		cmd->TransitionImageLayout(_viewportDepth, Runtime::RHI::ImageLayout::DepthAttachment);

		Runtime::RHI::ColorAttachment color{};
		color.image = _viewportColor;
		color.clearColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);

		Runtime::RHI::RenderingSubmitInfo clear{};
		clear.colorAttachments = { color };
		clear.depthAttachment = _viewportDepth;
		clear.extent = glm::vec2(static_cast<f32>(_viewportWidth), static_cast<f32>(_viewportHeight));
		cmd->BeginDynamicRendering(clear);
		cmd->EndDynamicRendering();

		return true;
	}

	void SceneRenderer2DSystem::CreateOffscreen(GraphicsSubsystem* gfx, u32 width, u32 height) {
		auto* context = gfx->GetContext();
		auto* swapchain = gfx->GetSwapchain();

		Runtime::RHI::ImageDesc colorDesc{};
		colorDesc.width = width;
		colorDesc.height = height;
		colorDesc.format = swapchain->GetColorFormat();
		colorDesc.usage = Runtime::RHI::ImageUsage::ColorAttachment | Runtime::RHI::ImageUsage::Sampled;
		colorDesc.view.type = Runtime::RHI::ImageViewType::Image2D;
		colorDesc.tileMode = Runtime::RHI::ImageTileMode::Optimal;
		colorDesc.sampler = Runtime::RHI::StaticSampler::LinearClamp;
		_viewportColor = context->CreateImage(colorDesc);

		Runtime::RHI::ImageDesc depthDesc = colorDesc;
		depthDesc.format = swapchain->GetDepthAttachment()->GetFormat();
		depthDesc.usage = Runtime::RHI::ImageUsage::DepthStencilAttachment;
		_viewportDepth = context->CreateImage(depthDesc);

		_viewportWidth = width;
		_viewportHeight = height;
		_viewportTexture = gfx->GetImGuiContext()->AddTexture(_viewportColor);
	}

	void SceneRenderer2DSystem::RetireOffscreen() {
		if (!_viewportColor) return;

		RetiredTarget r{};
		r.color = _viewportColor;
		r.depth = _viewportDepth;
		r.texture = _viewportTexture;
		r.frame = _offscreenFrame;
		_retiredTargets.push_back(r);

		_viewportColor = nullptr;
		_viewportDepth = nullptr;
		_viewportTexture = ImTextureID_Invalid;
		_viewportWidth = _viewportHeight = 0;
	}

	void SceneRenderer2DSystem::CollectRetired(GraphicsSubsystem* gfx, bool force) {
		const u64 delay = GraphicsSubsystem::MaxFramesInFlight + 1;
		for (auto it = _retiredTargets.begin(); it != _retiredTargets.end();) {
			if (force || _offscreenFrame >= it->frame + delay) {
				gfx->GetImGuiContext()->RemoveTexture(it->texture);
				Runtime::Mem::Allocator::Destroy(it->color);
				Runtime::Mem::Allocator::Destroy(it->depth);
				it = _retiredTargets.erase(it);
			}
			else {
				++it;
			}
		}
	}
}