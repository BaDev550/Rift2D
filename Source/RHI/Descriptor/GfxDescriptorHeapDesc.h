#pragma once

#include <Core/Types.h>
#include <RHI/Descriptor/GfxDescriptorBinding.h>
#include <vector>

namespace RHI {
	enum class GfxDescriptorHeapCreateBit : u8 {
		None = 0,
		UpdateAfterBind = BIT(1),
		FreeDescriptorSet = BIT(2)
	};
	using GfxDescriptorHeapCreateFlags = Flags<GfxDescriptorHeapCreateBit>;
	
	struct GfxDescriptorHeapSize {
		GfxDescriptorType type = GfxDescriptorType::SampledImage;
		u32 count = 0;
	};

	struct GfxDescriptorHeapDesc {
		u32 maxDescriptorSetAllocation = 1000;
		GfxDescriptorHeapCreateFlags flags = GfxDescriptorHeapCreateFlags::None;

		std::vector<GfxDescriptorHeapSize> sizes;
	};
}