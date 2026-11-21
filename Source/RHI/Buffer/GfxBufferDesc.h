#pragma once

#include <RHI/Buffer/GfxBufferUsage.h>
#include <RHI/Buffer/GfxMemoryUsage.h>

namespace RHI {
	struct GfxBufferDesc {
		usize size = 0;
		BufferUsage usage = BufferUsage::None;
		MemoryUsage memoryUsage = MemoryUsage::CPU;
		AllocationFlagBits allocationFlags = AllocationFlagBits::None;
	};
}