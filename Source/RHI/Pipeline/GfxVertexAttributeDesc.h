#pragma once
#include <Core/Types.h>
#include <RHI/Common/GfxFormat.h>
#include <vector>

namespace RHI {
	struct GfxVertexInputDesc {
		std::vector<GfxFormat> attribInputs;
		usize stride = 0;

		GfxVertexInputDesc() = default;
		GfxVertexInputDesc(const std::initializer_list<Format>& attribInputs) : attribInputs(attribInputs) {}
	};
}