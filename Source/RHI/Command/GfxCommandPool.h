#pragma once

#include <RHI/Object/GfxObject.h>

namespace RHI {
	class GfxCommandBuffer;
	struct GfxCommandBufferDesc;

	class GfxCommandPool : public Object {
	public:
		using Object::Object;
		virtual ~CommandPool() = default;

		virtual CommandBuffer* AllocateCommandBuffer(const CommandBufferDesc& desc) = 0;
		virtual void FreeCommandBuffer(CommandBuffer* commandBuffer) = 0;

		virtual void Reset() = 0;
	};
}