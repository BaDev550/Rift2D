#pragma once
#include <Core/Types.h>
#include <Runtime/Definitions/Flags.h>

namespace RHI {
	enum class GfxImageUsageBit : u16 {
		None = 0,
		Sampled = BIT(0),
		Storage = BIT(1),
		ColorAttachment = BIT(2),
		DepthStencilAttachment = BIT(3),
		TransferSrc = BIT(4),
		TransferDst = BIT(5)
	};
	using GfxImageUsageFlags = Flags<GfxImageUsageBit>;
}