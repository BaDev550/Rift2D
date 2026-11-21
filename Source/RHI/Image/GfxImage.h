#pragma once

#include <RHI/Object/GfxObject.h>
#include <RHI/Image/GfxImageDesc.h>

namespace RHI {
	class Image : public Object {
	public:
		using Object::Object;
		virtual ~Image() = default;

		virtual const ImageDesc* GetDesc() const = 0;
		virtual const Format GetFormat() const = 0;
		virtual const u32 GetWidth() const noexcept = 0;
		virtual const u32 GetHeight() const noexcept = 0;
		virtual u32 GetBindlessIndex() noexcept = 0;
		virtual u32 GetSamplerIndex() noexcept = 0;
		virtual StaticSampler GetStaticSampler() const noexcept = 0;
	};
}