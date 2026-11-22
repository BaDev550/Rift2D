#pragma once

#include <Core/Types.h>

namespace RHI {
	struct GfxPushConstantRange {
		usize size;
		usize offset;
	};
}