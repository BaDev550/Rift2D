#pragma once

#include <Core/Types.h>

namespace RHI {
	enum class GfxImageOp : u8 {
		Load,
		Clear,
		DontCare
	};
}