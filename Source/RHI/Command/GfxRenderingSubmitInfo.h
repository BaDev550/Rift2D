#pragma once

#include <RHI/Image/GfxLoadOp.h>
#include <Runtime/Math/Vector4.h>
#include <Runtime/Math/Vector3.h>
#include <Runtime/Math/Vector2.h>
#include <vector>

namespace RHI {
	class GfxImage;

	struct GfxPassDebugInfo {
		std::string label = "RenderPass";
		Math::Vec3 color = Math::Vec3(1.0f);
	};

	struct GfxAttachment {
		Image* image = nullptr;
		ImageLoadOp loadOp = ImageLoadOp::Clear;
		Math::Vec4 clearColor = Math::Vec4(1.0f);
		
		Attachment() = default;
		Attachment(Image* img, const Math::Vec4& clearCol = Math::Vec4(1.0f)) : image(img), clearColor(clearCol) {}
	};

	struct GfxRenderingSubmitInfo {
		std::vector<Attachment> colorAttachments;
		Attachment depthAttachment;

		PassDebugInfo* debugInfo = nullptr;

		Math::Vec2 extent;
	};
}