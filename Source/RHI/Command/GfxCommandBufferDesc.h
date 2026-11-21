#pragma once

#include <Runtime/Definitions/Types.h>

namespace RHI {
	enum class GfxCommandBufferType : u8 {
		Primary = 0,
		Secondary
	};

	struct GfxCommandBufferDesc {
		CommandBufferType type = CommandBufferType::Primary;
	};
}