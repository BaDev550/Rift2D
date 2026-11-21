#pragma once
#include <Engine/Core/Core.h>

namespace RHI {
	enum class ImageLayout : u8 {
		Undefined = 0,
		UnorderedAccess,
		PresentSrc,
		TransferDst,
		TransferSrc,
		ColorAttachment,
		DepthAttachment,
		ShaderReadOnly
	};
}