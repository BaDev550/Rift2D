#include "RenderPass.h"

namespace Rift::Engine {
	void RenderPass::Submit(const RenderFrame& frame) {
		Runtime::RHI::CommandBuffer* cmd = frame.cmd;
		
		if (cmd) {
			Runtime::RHI::Attachment colorAttachment{};
			colorAttachment.image = frame.color;
			colorAttachment.loadOp = Runtime::RHI::ImageLoadOp::Load;
			colorAttachment.clearColor = Runtime::Math::Vec4(1.0f); // TEMP
			colorAttachment.initalLayout = _initalLayout;

			Runtime::RHI::Attachment depthAttachment{};
			depthAttachment.image = frame.depth;
			depthAttachment.loadOp = Runtime::RHI::ImageLoadOp::Clear;
			depthAttachment.initalLayout = Runtime::RHI::ImageLayout::DepthAttachment;

			Runtime::RHI::PassDebugInfo debug{};
			debug.label = GetClassName();
			debug.color = Runtime::Math::Vec3(1.0f);

			Runtime::RHI::RenderingSubmitInfo info{};
			info.colorAttachments = { colorAttachment };
			info.depthAttachment = depthAttachment;
			info.extent = frame.extent;
			info.debugInfo = &debug;

			cmd->BeginDynamicRendering(info);

			Execute(frame);
			cmd->EndDynamicRendering();
			cmd->TransitionImageLayout(frame.depth, _finalLayout);
		}
	}
}