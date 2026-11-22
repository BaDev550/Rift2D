#pragma once
#include <Core/Types.h>
#include <RHI/Image/GfxImage.h>

namespace RHI {
	enum class ImageViewType : u8 {
		None = 0,
		Image2D,
		Image3D
	};

	struct GfxImageViewDesc {
		ImageViewType type = ImageViewType::None;
		u32 mipCount = 0;
	};

	class GfxImageView {
	public:
		virtual ~GfxImageView() = default;

		[[nodiscard]] const GfxImageViewDesc& GetDesc() const { return _desc; }
	private:
		GfxImageViewDesc _desc;
	};
}