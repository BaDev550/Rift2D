#pragma once

#include <RHI/Pipeline/GfxPipelineDesc.h>
#include <RHI/Object/GfxObject.h>

namespace RHI {
	class CommandBuffer;

	class GfxPipeline : public GfxObject {
	public:
		using GfxObject::GfxObject;
		virtual ~GfxPipeline() = default;

		[[nodiscard]] virtual const GfxPipelineDesc& GetDesc() const = 0;
	};
}