#pragma once

#include <Runtime/Definitions/Types.h>
#include <Engine/Asset/Asset.h>

namespace Rift::Engine {
	class SpriteMaterial final {
	public:
		[[nodiscard]] AssetHandle& GetDiffuseHandle() { return _diffuseHandle; }
		[[nodiscard]] AssetHandle& GetNormalHandle() { return _normalHandle; }
	private:
		AssetHandle _diffuseHandle;
		AssetHandle _normalHandle;
	};
}