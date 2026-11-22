#pragma once

#include <RHI/Object/GfxObject.h>

namespace RHI {
	class GfxCommandBuffer;
	class GfxSwapchain;
	class GfxFence;

	class GfxQueue : public GfxObject {
	public:
		using GfxObject::GfxObject;
		virtual ~GfxQueue() = default;

		virtual void Wait() = 0;
		virtual void Submit(GfxCommandBuffer* commandBuffer, const std::vector<GfxFence*>& waitFences, const std::vector<GfxFence*>& signalFences) = 0;
		virtual void Present(GfxSwapchain* swapchain, u32 imageIndex, const std::vector<GfxFence*>& waitFences) = 0;

		[[nodiscard]] virtual u32 GetQueueFamilyIndex() const = 0;
	};
}