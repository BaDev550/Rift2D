#pragma once
#include <Core/Types.h>
#include <Runtime/Definitions/Flags.h>

namespace RHI {
	enum class GfxShaderStageBit : u8 {
		None = 0,
		Vertex = BIT(1),
		Fragment = BIT(2),
		Mesh = BIT(3),
		Compute = BIT(4),
		All = Vertex | Fragment | Mesh | Compute
	};
	using GfxShaderStageFlags = using Flags<GfxShaderStageBit>
}
