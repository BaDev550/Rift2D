#pragma once

#include <Core/Types.h>

namespace RHI {
	enum class GfxCommandPoolUsage : u8 {
		Transient = 0,
		Resettable
	};

	struct GfxCommandPoolDesc {
		u32 queueFamilyIndex = 0;
		CommandPoolUsage usage = CommandPoolUsage::Transient;
	};
}