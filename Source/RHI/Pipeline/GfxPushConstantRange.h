#pragma once

#include <Runtime/Definitions/Types.h>

namespace RHI {
	struct PushConstantRange {
		usize size;
		usize offset;
	};
}