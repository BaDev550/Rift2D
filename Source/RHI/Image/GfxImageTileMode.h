#pragma once
#include <Engine/Core/Core.h>

namespace RHI {
	enum class GfxImageTileMode : u8 {
		Undefined = 0,
		Repeat,
		Mirror,
		ClampToEdge,
		ClampToBorder,
		Optimal
	};
}