#pragma once
#include <Engine/Core/Core.h>

#include <RHI/Common/GfxFormat.h>
#include <RHI/Image/GfxImageUsage.h>
#include <RHI/Image/GfxImageLayout.h>
#include <RHI/Image/GfxImageTileMode.h>
#include <RHI/Image/GfxImageView.h>
#include <RHI/Image/GfxStaticSampler.h>

namespace RHI {
	struct ImageDesc {
		u32 width = 0;
		u32 height = 0;
		ImageView view;
		StaticSampler sampler = StaticSampler::NearestRepeat;
		Format format = Format::Undefined;
		ImageUsage usage = ImageUsage::None;
		ImageTileMode tileMode = ImageTileMode::Repeat;
	};
}