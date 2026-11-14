#pragma once 

#include <Runtime/Definitions/Types.h>

namespace Runtime::RHI {
	enum class CompareOp : u8 {
		Equal,
		NotEqual,
		Greater,
		Less,
		Trigger
	};

	enum class DepthTest : u8 {
		Enabled = 0,
		Disabled
	};
}