#pragma once

#include <Runtime/Definitions/Types.h>

namespace RHI {
	enum class QueueType : u8 {
		Graphics = 0,
		Compute,
		Transfer
	};
}