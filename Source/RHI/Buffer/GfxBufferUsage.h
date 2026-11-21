#pragma once
#include <Runtime/Definitions/Flags.h>

namespace RHI {
	enum class GfxBufferUsageBit : u16 {
		None = 0,
		Uniform = BIT(0),
		Storage = BIT(1),
		TransferSrc = BIT(2),
		TransferDst = BIT(3),
		DeviceAddress = BIT(4),
		Index = BIT(5),
		Vertex = BIT(6)
	};
	using GfxBufferUsageFlags = Flags<GfxBufferUsageBit>;
}