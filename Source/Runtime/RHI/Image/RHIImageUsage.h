#pragma once
#include <Engine/Core/Core.h>

namespace Runtime::RHI {
	enum class ImageUsage : u16 {
		None = 0,
		Sampled = BIT(0),
		Storage = BIT(1),
		ColorAttachment = BIT(2),
		DepthStencilAttachment = BIT(3),
		TransferSrc = BIT(4),
		TransferDst = BIT(5)
	};
	CORE_DEFINE_ENUM_FLAG_OPERATORS(ImageUsage);
}