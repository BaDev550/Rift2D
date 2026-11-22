#pragma once

#include <Core/Types.h>
#include <RHI/Descriptor/GfxDescriptorType.h>
#include <RHI/Descriptor/GfxDescriptorBinding.h>
#include <vector>

namespace RHI {
	struct GfxDescriptorSetAllocInfo {
		std::vector<GfxDescriptorBindingDesc> bindings;
	};
}