#pragma once
#include <Engine/Core/Core.h>

namespace RHI {
	enum class PresentMode {
		Instant = 0,
		Vsync,
		Mailbox
	};

	struct GfxSwapchainDesc {
		u32 width;
		u32 height;
		
		GfxFormat colorFormat = GfxFormat::RGBA8;
		GfxFormat depthFormat = GfxFormat::D32_SFLOAT;
		PresentMode presentMode = PresentMode::Instant;
	};
}