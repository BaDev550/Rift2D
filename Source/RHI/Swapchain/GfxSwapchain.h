#pragma once

#include <Core/Result.h>
#include <RHI/Object/GfxObject.h>
#include <RHI/Common/GfxFormat.h>
#include <RHI/Swapchain/GfxSwapchainDesc.h>

#include <glm/glm.hpp>

namespace RHI {
	class GfxImage;
	class GfxFence;

	class GfxSwapchain : public GfxObject {
	public:
		using GfxObject::GfxObject;
		virtual ~GfxSwapchain() = default;

		virtual void Recreate(const GfxSwapchainDesc& desc) = 0;
		virtual void Recreate(u32 width, u32 height) = 0;
		virtual void Destroy() = 0;
		virtual Result AcquireNextImage(u32& imageIndex, GfxFence* imageAvailableFence) = 0;

		virtual [[nodiscard]] glm::vec2 GetExtent() const = 0;
		virtual [[nodiscard]] u32 GetImageCount() const = 0;

		virtual [[nodiscard]] std::vector<GfxImage*>& GetColorAttachments() = 0;
		virtual [[nodiscard]] GfxImage* GetDepthAttachment() = 0;

		virtual [[nodiscard]] const GfxFormat GetColorFormat() const = 0;
		virtual [[nodiscard]] const GfxFormat GetDepthFormat() const = 0;
		virtual [[nodiscard]] const GfxSwapchainDesc& GetDesc() const = 0;
	};
}