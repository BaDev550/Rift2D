#include "ForwardPass.h"
#include <Runtime/Definitions/Types.h>
#include <Engine/Asset/AssetSubsystem.h>
#include <Engine/Platform/Window/WindowSubsystem.h>
#include <Engine/Graphics/GraphicsSubsystem.h>
#include <Engine/Asset/Shader/Shader.h>
#include <Runtime/Definitions/Profiler.h>

namespace Rift::Engine {

	void ForwardRenderPass::OnInitialize() {
		auto assetSystem = GetEngine()->GetSubsystem<AssetSubsystem>();
		auto* renderSystem = GetEngine()->GetSubsystem<GraphicsSubsystem>();
		{
			Runtime::RHI::ImageDesc colorDesc{};
			colorDesc.width = renderSystem->GetSwapchain()->GetExtent().x;
			colorDesc.height = renderSystem->GetSwapchain()->GetExtent().y;
			colorDesc.format = Runtime::RHI::Format::RGBA8;
			colorDesc.usage = Runtime::RHI::ImageUsage::ColorAttachment;
			_color = renderSystem->GetContext()->CreateImage(colorDesc);

			Runtime::RHI::ImageDesc depthDesc{};
			depthDesc.width = renderSystem->GetSwapchain()->GetExtent().x;
			depthDesc.height = renderSystem->GetSwapchain()->GetExtent().y;
			depthDesc.format = Runtime::RHI::Format::D32_SFLOAT;
			depthDesc.usage = Runtime::RHI::ImageUsage::DepthStencilAttachment;
			_depth = renderSystem->GetContext()->CreateImage(depthDesc);
		}
		{
			Shader* forwardPassVertexShader = assetSystem->Get<Shader>("Shaders/2D_forward_vert.vert");
			Shader* forwardPassFragmentShader = assetSystem->Get<Shader>("Shaders/2D_forward_frag.frag");

			Runtime::RHI::PushConstantRange pcRange{};
			pcRange.size = sizeof(SpritePushConstant);
			pcRange.offset = 0;

			Runtime::RHI::PipelineDesc vertexDesc{};
			vertexDesc.colorFormats = { renderSystem->GetSwapchain()->GetColorFormat() };
			vertexDesc.depthFormat = renderSystem->GetSwapchain()->GetDepthFormat();
			vertexDesc.vertexShader = &forwardPassVertexShader->GetDesc();
			vertexDesc.fragmentShader = &forwardPassFragmentShader->GetDesc();
			vertexDesc.pushConstantRanges = { pcRange };
			vertexDesc.blending = true;
			vertexDesc.depthTest = false;

			_forwardPassPipeline = renderSystem->GetContext()->CreatePipeline(vertexDesc);
		}
		{
			Runtime::RHI::BufferDesc sbDesc{};
			sbDesc.size = sizeof(SpriteData) * MaxSprites;
			sbDesc.usage = Runtime::RHI::BufferUsage::Storage | Runtime::RHI::BufferUsage::DeviceAddress;
			sbDesc.memoryUsage = Runtime::RHI::MemoryUsage::Auto;
			sbDesc.allocationFlags = Runtime::RHI::AllocationFlagBits::HostAccessRandom | Runtime::RHI::AllocationFlagBits::CreateMapped;
			_spriteDataStorageBuffer = renderSystem->GetContext()->CreateBuffer(sbDesc);
		}
	}

	void ForwardRenderPass::Setup(RenderGraphBuilder& builder) {
		ResourceHandle forwardColorHandle = builder.Import("ForwardColor", _color);
		ResourceHandle forwardDepthHandle = builder.Import("ForwardDepth", _depth);
		builder.Write(forwardColorHandle, Runtime::RHI::ImageLayout::ColorAttachment);
		builder.Write(forwardDepthHandle, Runtime::RHI::ImageLayout::DepthAttachment);
	}

	void ForwardRenderPass::Execute(const RenderFrame& frame, const RenderGraphResourceContainer& resources) {
		auto* renderSystem = GetEngine()->GetSubsystem<GraphicsSubsystem>();

		{
			BEGIN_PROFILE_SCOPE("ForwardPass");

			Runtime::RHI::Attachment colorAttachment{};
			colorAttachment.image = _color;
			colorAttachment.loadOp = Runtime::RHI::ImageLoadOp::Load;
			colorAttachment.clearColor = Runtime::Math::Vec4(1.0f, 1.0f, 1.0f, 1.0f);

			Runtime::RHI::Attachment depthAttachment{};
			colorAttachment.image = _depth;
			colorAttachment.loadOp = Runtime::RHI::ImageLoadOp::Clear;

			Runtime::RHI::PassDebugInfo debug{};
			debug.label = "ForwardPass";
			debug.color = Runtime::Math::Vec3(1.0f, 0.0f, 0.0f);

			Runtime::RHI::RenderingSubmitInfo info{};
			info.colorAttachments = { colorAttachment };
			info.depthAttachment = depthAttachment;
			info.extent = frame.extent;
			info.debugInfo = &debug;

			frame.cmd->BeginDynamicRendering(info);
			frame.cmd->BindPipeline(_forwardPassPipeline);

			auto* bindlessSet = renderSystem->GetContext()->GetBindlessDescriptorSet();
			auto* globalSet = renderSystem->GetContext()->GetGlobalDescriptorSet();

			frame.cmd->BindDescriptorSet(_forwardPassPipeline, bindlessSet, 0, 1);
			frame.cmd->BindDescriptorSet(_forwardPassPipeline, globalSet, 0, 0);

			auto spriteQueue = frame.GetDrawQueue(RenderQueue::Sprite);
			if (!spriteQueue.empty()) {
				_spriteDatas.resize(spriteQueue.size());

				int i = 0;
				for (const RenderItem& item : spriteQueue) {
					SpriteData spriteData{};
					spriteData.world = item.world;
					spriteData.vertexBufferID = item.vertexBufferIndex;
					spriteData.firstVertex = item.firstVertex;
					spriteData.frameDataIndex = frame.frameDataBufferIndex;
					spriteData.diffuseTextureID = item.textureID;
					spriteData.normalTextureID = 0;
					spriteData.samplerID = item.samplerID;

					_spriteDatas[i] = spriteData;
					i++;
				}

				if (!_spriteDatas.empty()) {
					_spriteDataStorageBuffer->Write(_spriteDatas.data(), (sizeof(SpriteData) * _spriteDatas.size()));

					SpritePushConstant pc{};
					pc.spriteDataBDA = _spriteDataStorageBuffer->GetGPUAdress();

					frame.cmd->PushConstant(_forwardPassPipeline, &pc, sizeof(SpritePushConstant), 0);
					frame.cmd->DrawVertex(6, spriteQueue.size());
				}
			}
			frame.cmd->EndDynamicRendering();
		}
	}

	void ForwardRenderPass::OnDestroy() {
		Runtime::Mem::Allocator::Destroy(_spriteDataStorageBuffer);
		Runtime::Mem::Allocator::Destroy(_forwardPassPipeline);
	}
}