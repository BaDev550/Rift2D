#pragma once

#include <RHI/Object/GfxObject.h>
#include <RHI/Image/GfxImageDesc.h>

namespace RHI {
	class GfxImage : public GfxObject {
	public:
		using GfxObject::GfxObject;
		virtual ~GfxImage() = default;

		virtual const GfxImageDesc* GetDesc() const = 0;
		virtual const GfxFormat GetFormat() const = 0;
		virtual const u32 GetWidth() const noexcept = 0;
		virtual const u32 GetHeight() const noexcept = 0;
		virtual u32 GetBindlessIndex() noexcept = 0;
		virtual u32 GetSamplerIndex() noexcept = 0;
		virtual GfxStaticSampler GetStaticSampler() const noexcept = 0;
	};
}