#pragma once

#include <Engine/Core/Core.h>

namespace RHI {
	enum class GfxStaticSampler : u8 {
		PointClamp = 0,
		PointWrap,
		LinearClamp,
		LinearWrap,
		LinearMirror,
		NearestClamp,
		NearestRepeat,
		AnisoClamp,
		AnisoMirror,
		ShadowCompare,
		COUNT
	};
}