#pragma once

#include <Runtime/Definitions/Types.h>
#include <Runtime/Definitions/Flags.h>
#include <RHI/Descriptor/GfxDescriptorType.h>
#include <RHI/Shader/ShaderStage.h>

namespace RHI {
	enum class DescriptorBindingBit : u8 {
		None = 0,
		PartiallyBound = BIT(1),
		UpdateAfterBind = BIT(2)
	};
	using DescriptorBindingFlags = Flags<DescriptorBindingBit>;
	
	struct DescriptorBindingDesc {
		u32 count = 1;
		DescriptorType type = DescriptorType::SampledImage;
		ShaderStage stage = ShaderStage::All;
		DescriptorBindingFlags flags = DescriptorBindingFlags::None;
	private:
		u32 binding = 0;

		friend class DescriptorHeap;
		friend class DescriptorSetLayout;
	};
}