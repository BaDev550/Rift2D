#pragma once
#include <Runtime/Definitions/Flags.h>

namespace RHI {
	enum class GfxMemoryUsage : u8 {
		Auto,
		GPU,
		CPU,
		CPUToGPU
	};

	enum class GfxAllocationBit : u8 {
		None = 0,
		HostAccessSequentialWrite = BIT(0),
		HostAccessRandom = BIT(1),
		CreateMapped = BIT(2)
	};
	using GfxAllocationFlags = Flags<GfxAllocationBit>;
}