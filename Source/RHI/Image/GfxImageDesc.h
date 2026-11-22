#pragma once
#include <Engine/Core/Core.h>

#include <RHI/Common/GfxFormat.h>
#include <RHI/Image/GfxImageUsage.h>
#include <RHI/Image/GfxImageLayout.h>
#include <RHI/Image/GfxImageTileMode.h>
#include <RHI/Image/GfxImageView.h>
#include <RHI/Image/GfxStaticSampler.h>

namespace RHI {
	struct GfxImageDesc {
		u32 width = 0;
		u32 height = 0;
		GfxFormat format = GfxFormat::Undefined;
		GfxImageViewDesc* viewDesc;
		GfxStaticSampler sampler = GfxStaticSampler::NearestRepeat;
		GfxImageUsageFlags usage = GfxImageUsageFlags::None;
		GfxImageTileMode tileMode = GfxImageTileMode::Repeat;
	};
}