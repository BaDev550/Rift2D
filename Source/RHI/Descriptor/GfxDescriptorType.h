#pragma once

#include <Runtime/Definitions/Types.h>

namespace RHI {
	enum class DescriptorType : u8 {
		Sampler,
		SampledImage,
		StorageImage,
		UniformBuffer,
		StorageBuffer
	};
}