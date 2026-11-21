#pragma once

#include <Runtime/Definitions/Types.h>
#include <RHI/Descriptor/GfxDescriptorType.h>
#include <RHI/Descriptor/GfxDescriptorBinding.h>
#include <vector>

namespace RHI {
	struct DescriptorSetAllocInfo {
		std::vector<DescriptorBindingDesc> bindings;
	};
}