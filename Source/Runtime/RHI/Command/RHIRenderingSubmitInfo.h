#pragma once

#include <vector>
#include <Runtime/RHI/Image/RHILoadOp.h>
#include <Runtime/Math/Vector4.h>
#include <Runtime/Math/Vector3.h>
#include <Runtime/Math/Vector2.h>

namespace Runtime::RHI {
	class Image;

	struct PassDebugInfo {
		std::string label = "RenderPass";
		Runtime::Math::Vec3 color = Runtime::Math::Vec3(1.0f);
	};

	struct Attachment {
		Image* image = nullptr;
		ImageLoadOp loadOp = ImageLoadOp::Clear;
		Runtime::Math::Vec4 clearColor = Runtime::Math::Vec4(1.0f);
		Runtime::RHI::ImageLayout initalLayout = Runtime::RHI::ImageLayout::Undefined;
		Runtime::RHI::ImageLayout finalLayout = Runtime::RHI::ImageLayout::Undefined;

		Attachment() = default;
		Attachment(Image* img, const Runtime::Math::Vec4& clearCol = Runtime::Math::Vec4(1.0f)) : image(img), clearColor(clearCol) {}
	};

	struct RenderingSubmitInfo {
		std::vector<Attachment> colorAttachments;
		Attachment depthAttachment;
		PassDebugInfo* debugInfo = nullptr;

		Runtime::Math::Vec2 extent;
	};
}