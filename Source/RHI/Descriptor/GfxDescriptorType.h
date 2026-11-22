#pragma once

#include <Core/Types.h>

namespace RHI {
	enum class GfxDescriptorType : u8 {
		Sampler,
		SampledImage,
		StorageImage,
		UniformBuffer,
		StorageBuffer
	};
}