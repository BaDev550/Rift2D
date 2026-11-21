#pragma once
#include <Runtime/Definitions/Types.h>
#include <RHI/Common/GfxFormat.h>
#include <vector>

namespace RHI {
	struct VertexInputDesc {
		std::vector<Format> attribInputs;
		usize stride = 0;

		VertexInputDesc() = default;
		VertexInputDesc(const std::initializer_list<Format>& attribInputs) : attribInputs(attribInputs) {}
	};
}