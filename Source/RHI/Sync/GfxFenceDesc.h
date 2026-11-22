#pragma once

#include <Core/Types.h>

namespace RHI {
	enum class GfxSemaphoreType : u8 {
		Binary = 0,
		Timeline
	};

	struct GfxFenceDesc {
		GfxSemaphoreType type = GfxSemaphoreType::Binary;
		int32 initialValue = 0;
	};
}