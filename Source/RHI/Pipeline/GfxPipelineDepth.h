#pragma once 

#include <Core/Types.h>

namespace RHI {
	enum class GfxCompareOp : u8 {
		Equal,
		NotEqual,
		Greater,
		Less,
		Trigger
	};

	enum class GfxDepthTest : u8 {
		Enabled = 0,
		Disabled
	};
}