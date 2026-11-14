#pragma once

#include <Engine/World/System.h> // TODO(0x): move this to another folder
#include <Engine/Asset/Texture/Texture2D.h>
#include <Engine/Render/Passes/ForwardPass.h>
#include <Engine/Render/Passes/OccluderPass.h>
#include <Engine/Render/Passes/LightPass.h>
#include <Engine/Graphics/GraphicsSubsystem.h>

#include <Runtime/RHI/Pipeline/RHIPipeline.h>

namespace Rift::Engine {
	class ENGINE_API SceneRenderer2DSystem final : public System {
	public:
		virtual Runtime::Result OnCreate() override;
		virtual void OnUpdate(Scene& scene) override;
		virtual void OnDestroy() override;

		void SetViewportSize(u32 width, u32 height) { _viewportActive = true; _viewportRequestW = width; _viewportRequestH = height; }
		ImTextureID GetViewportTexture() const { return _viewportTexture; }
	private:
		bool BeginOffscreen(GraphicsSubsystem* gfx, Runtime::RHI::CommandBuffer* cmd);
		void CreateOffscreen(GraphicsSubsystem* gfx, u32 width, u32 height);
		void RetireOffscreen();
		void CollectRetired(GraphicsSubsystem* gfx, bool force);

		ForwardRenderPass* _forwardRenderPass;
		OccluderPass* _occluderPass;
		LightPass* _lightPass;

		Runtime::RHI::Image* _viewportColor = nullptr;
		Runtime::RHI::Image* _viewportDepth = nullptr;
		ImTextureID _viewportTexture = ImTextureID_Invalid;
		u32 _viewportWidth = 0, _viewportHeight = 0;
		bool _viewportActive = false;
		u32 _viewportRequestW = 1, _viewportRequestH = 1;
		u64 _offscreenFrame = 0;

		struct RetiredTarget {
			Runtime::RHI::Image* color = nullptr;
			Runtime::RHI::Image* depth = nullptr;
			ImTextureID texture = ImTextureID_Invalid;
			u64 frame = 0;
		};
		std::vector<RetiredTarget> _retiredTargets;

		Runtime::RHI::Buffer* _frameDataBuffers[GraphicsSubsystem::MaxFramesInFlight];
		Runtime::RHI::Buffer* _lightBuffers[GraphicsSubsystem::MaxFramesInFlight];
		Runtime::RHI::Buffer* _spriteBuffer;
	};
}