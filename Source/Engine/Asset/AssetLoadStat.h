#pragma once
#include <Engine/Core/Core.h>

namespace Rift::Engine {
	enum class AssetState : u8 {
		InDisk,
		Loading,
		Loaded,
		Ready
	};
}