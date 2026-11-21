#pragma once

#include <Engine/Render/RenderPass.h>
#include <Engine/Render/Sprite/SpriteData.h>
#include <Engine/Render/Sprite/SpriteMaterial.h>

#include <Runtime/RHI/Pipeline/RHIPipeline.h>
#include <Runtime/Math/Mat4.h>

namespace Rift::Engine {
	RCLASS();
	class ENGINE_API ForwardRenderPass final : public RenderPass {
		GENERATE_CLASS(ForwardRenderPass)
	public:
		virtual void OnInitialize() final;
		virtual void OnDestroy() final;
		virtual void Setup(RenderGraphBuilder& builder) final;
		virtual void Execute(const RenderFrame& frame, const RenderGraphResourceContainer& resources) final;

		virtual i32 GetOrder() const final { return 0; }
		virtual RenderStage GetStage() const final { return RenderStage::Scene; }
	private:
		Runtime::RHI::Image* _color;
		Runtime::RHI::Image* _depth;

		std::vector<SpriteData> _spriteDatas;
		Runtime::RHI::Buffer* _spriteDataStorageBuffer = nullptr;
		Runtime::RHI::Pipeline* _forwardPassPipeline = nullptr;
	};
}