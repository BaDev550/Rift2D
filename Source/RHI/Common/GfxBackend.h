#pragma once
#include <Engine/Core/Core.h>

namespace RHI {
	enum class GfxBackend : u8 {
		None = 0,
		Vulkan = 1,
		D3D12 = 2
	};
}