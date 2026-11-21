#pragma once

#include <RHI/Object/GfxObject.h>
#include <RHI/Image/GfxImageLayout.h>
#include <RHI/Pipeline/GfxPipelineBindPoint.h>
#include <RHI/Command/GfxRenderingSubmitInfo.h>

namespace RHI {
	class GfxQueue;
	class GfxImage;
	class GfxBuffer;
	class GfxPipeline;
	class GfxDescriptorSet;

	class GfxCommandBuffer : public Object {
	public:
		using Object::Object;
		virtual ~CommandBuffer() = default;

		virtual void Begin() = 0;
		virtual void End() = 0;
		virtual void Reset() = 0;
		
		virtual void BeginDynamicRendering(const RenderingSubmitInfo& info) = 0;
		virtual void EndDynamicRendering() = 0;

		virtual void TransitionImageLayout(Image* image, ImageLayout newLayout) = 0;
		virtual void CopyBuffer(Buffer* srcBuffer, Buffer* dstBuffer, size_t size) = 0;
		virtual void CopyBufferToImage(Buffer* buffer, Image* image, ImageLayout layout, u32 width, u32 height) = 0;

		virtual void DrawVertex(u32 count, u32 instances = 1) = 0;

		virtual void BindPipeline(Pipeline* pipeline, PipelineBindPoint point = PipelineBindPoint::Graphics) = 0;
		virtual void PushConstant(Pipeline* pipeline, void* data, usize size, usize offset = 0) = 0;
		virtual void BindDescriptorSet(Pipeline* pipeline, DescriptorSet* descriptor, u32 binding, u32 set) = 0;
		virtual void Dispatch(u32 groupX, u32 groupY, u32 groupZ) = 0;
	};
}