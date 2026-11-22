#pragma once

#include <Engine/Asset/AssetSource.h>

#include <Core/Result.h>
#include <Runtime/Reflection/ReflectObject.h>

namespace Rift::Engine {
	class Asset;

	RCLASS();
	class AssetStreamer : public Runtime::Reflection::ReflectObject {
		GENERATE_CLASS(AssetStreamer);
	public:
		virtual ~AssetStreamer() = default;

		virtual Runtime::Result Cook(AssetSource& source, void* settings = nullptr) { return Runtime::Result(); }
		virtual Asset* Import(AssetSource& source) { return nullptr; }

		virtual std::string GetAssetClassName() { return "Asset"; };
	};
}