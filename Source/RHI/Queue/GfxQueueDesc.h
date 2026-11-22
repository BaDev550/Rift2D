#pragma once

#include <RHI/Queue/GfxQueueType.h>

namespace RHI {
	struct GfxQueueDesc {
		GfxQueueType type = GfxQueueType::Graphics;
	};
}