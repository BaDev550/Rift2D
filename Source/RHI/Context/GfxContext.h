#pragma once
#include <RHI/Context/GfxContextDesc.h>

struct ImGuiContext;
namespace RHI {
	class GfxImage;
	class GfxBuffer;
	class GfxSwapchain;
	class GfxCommandBuffer;
	class GfxCommandPool;
	class GfxQueue;
	class GfxFence;
	class GfxPipeline;
	class GfxDescriptorHeap;
	class GfxDescriptorSet;
	class GfxImGuiContext;
	struct GfxImageDesc;
	struct GfxBufferDesc;
	struct GfxSwapchainDesc;
	struct GfxCommandBufferDesc;
	struct GfxCommandPoolDesc;
	struct GfxQueueDesc;
	struct GfxFenceDesc;
	struct GfxPipelineDesc;
	struct GfxDescriptorHeapDesc;

	class GfxContext {
	public:
		GfxContext() = default;
		virtual ~GfxContext() = default;

		virtual void WaitDeviceIdle() = 0;
		virtual void Shutdown() = 0;

		virtual GfxBuffer* CreateBuffer(const GfxBufferDesc& desc) = 0;
		virtual GfxImage* CreateImage(const GfxImageDesc& desc) = 0;
		virtual GfxSwapchain* CreateSwapchain(const GfxSwapchainDesc& desc) = 0;
		virtual GfxCommandPool* CreateCommandPool(const GfxCommandPoolDesc& desc) = 0;
		virtual GfxQueue* CreateQueue(const GfxQueueDesc& desc) = 0;
		virtual GfxFence* CreateFence(const GfxFenceDesc& desc) = 0;
		virtual GfxPipeline* CreatePipeline(const GfxPipelineDesc& desc) = 0;
		virtual GfxDescriptorHeap* CreateDescriptorHeap(const GfxDescriptorHeapDesc& desc) = 0;
		virtual GfxImGuiContext* CreateImGuiContext() = 0;

		virtual void SetFallbackImage(RHI::GfxImage* image) = 0;

		virtual GfxQueue* GetGraphicsQueue() = 0;
		virtual GfxContextDesc* GetDesc() = 0;
		virtual GfxDescriptorSet* GetBindlessDescriptorSet() = 0;
		virtual GfxDescriptorSet* GetGlobalDescriptorSet() = 0;
	};
}

extern "C" {
	RUNTIME_API GfxContext* CreateContext(const GfxContextDesc& desc);
}