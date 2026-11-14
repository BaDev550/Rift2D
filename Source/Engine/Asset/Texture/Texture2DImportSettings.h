#pragma once

#include <Runtime/Definitions/Types.h>
#include <Runtime/RHI/Common/RHIFormat.h>
#include <Runtime/RHI/Image/RHIImageDesc.h>

namespace Rift::Engine {
	struct TextureImportSettings {
		Runtime::RHI::Format format = Runtime::RHI::Format::BC3;
		Runtime::RHI::StaticSampler sampler = Runtime::RHI::StaticSampler::NearestClamp;
		Runtime::RHI::ImageTileMode tile = Runtime::RHI::ImageTileMode::Optimal;
		b8 flipVertical = false;
		b8 isTileMap = false;
		u32 tileWidth = 32;
		u32 tileHeight = 32;
	};
}