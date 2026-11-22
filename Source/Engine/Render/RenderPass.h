#pragma once

#include <Engine/Core/Engine.h>
#include <Core/Types.h>
#include <Runtime/Reflection/ReflectObject.h>
#include <Runtime/RHI/Image/RHIImageLayout.h>
#include <Engine/Render/RenderFrame.h>
#include <Engine/Render/RenderQueue.h>
#include <Engine/Render/RenderGraphBuilder.h>

namespace Rift::Engine {
	class GraphicsSubsystem;
	class RenderGraphResourceContainer;

	enum class RenderStage : i32 {
		PreScene = 0,
		Scene = 1000,
		PostProcess = 2000,
		UI = 3000,
		Final = 4000
	};

	RCLASS();
	class RenderPass : public Runtime::Reflection::ReflectObject {
		GENERATE_CLASS(RenderPass)
	public:
		virtual ~RenderPass() = default;

		virtual void OnInitialize() = 0;
		virtual void OnDestroy() = 0;
		virtual void Setup(RenderGraphBuilder& builder) = 0;
		virtual void Execute(const RenderFrame& frame, const RenderGraphResourceContainer& resources) = 0;

		virtual i32 GetOrder() const { return 0; }
		virtual RenderStage GetStage() const { return RenderStage::Scene; }

		void SetEngine(Engine* engine) { _engine = engine; }
	protected:
		Engine* GetEngine() const { return _engine; }
	private:
		Engine* _engine;

		i32 GetShortingValue() const { return (GetOrder() + static_cast<i32>(GetStage())); }

		friend class RenderGraph;
		friend class GraphicsSubsystem;
	};
}