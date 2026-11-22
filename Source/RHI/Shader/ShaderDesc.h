#pragma once
#include <Engine/Core/Core.h>

namespace RHI {
	struct ShaderDesc { // TODO(0x): find a decent way
		std::vector<u32> code;
		usize size = 0;
	};
}