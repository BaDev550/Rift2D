#pragma once

#include <Core/Types.h>

namespace RHI {
	enum class GfxQueueType : u8 {
		Graphics = 0,
		Compute,
		Transfer
	};
}