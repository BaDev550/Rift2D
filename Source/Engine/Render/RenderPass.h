#pragma once

#include <Engine/Core/Engine.h>
#include <Runtime/Definitions/Types.h>
#include <Runtime/Reflection/ReflectObject.h>
#include <Runtime/RHI/Image/RHIImageLayout.h>
#include <Engine/Render/RenderFrame.h>

namespace Rift::Engine {
	class GraphicsSubsystem;

	enum class RenderStage : i32 {
		PreScene = 0,
		Scene = 1000,
		PostProcess = 2000,
		UI = 3000,
		Final = 4000
	};

	RCLASS();
	class ENGINE_API RenderPass : public Runtime::Reflection::ReflectObject {
		GENERATE_CLASS(RenderPass)
	public:
		virtual ~RenderPass() = default;

		void Submit(const RenderFrame& frame);

		virtual void OnInitialize() = 0;
		virtual void OnDestroy() = 0;
		virtual void Execute(const RenderFrame& frame) = 0;

		virtual i32 GetOrder() const { return 0; }
		virtual RenderStage GetStage() const { return RenderStage::Scene; }

		void SetEngine(Engine* engine) { _engine = engine; }
	protected:
		Engine* GetEngine() const { return _engine; }
	private:
		Engine* _engine;
		Runtime::RHI::ImageLayout _initalLayout = Runtime::RHI::ImageLayout::Undefined;
		Runtime::RHI::ImageLayout _finalLayout = Runtime::RHI::ImageLayout::Undefined;
		
		friend class GraphicsSubsystem;
	};
}