#pragma once

#include <RHI/Pipeline/GfxPipelineDesc.h>
#include <RHI/Object/GfxObject.h>

namespace RHI {
	class CommandBuffer;

	class Pipeline : public Object {
	public:
		using Object::Object;
		virtual ~Pipeline() = default;

		[[nodiscard]] virtual const PipelineDesc& GetDesc() const = 0;
	};
}