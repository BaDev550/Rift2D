#pragma once
#include <Engine/Core/Core.h>

namespace RHI {
	struct ShaderDesc {
		std::vector<u32> code;
		usize size = 0;
	};
}