#pragma once

#include <Engine/Asset/AssetStreamer.h>
#include <Engine/Asset/Texture/Texture2D.h>

namespace Rift::Engine {
    RCLASS();
	class ENGINE_API Texture2DStreamer final : public AssetStreamer {
        GENERATE_CLASS(Texture2DStreamer);
	public:
		virtual Runtime::Result Cook(AssetSource& source, void* settings = nullptr) override;
        virtual Asset* Import(AssetSource& source) override;

        virtual std::string GetAssetClassName() override { return Texture2D::GetStaticClassName().data(); };
	};
}