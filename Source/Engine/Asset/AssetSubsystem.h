#pragma once
#include <Engine/Core/Core.h>
#include <Engine/Subsystem/Subsystem.h>

#include <Engine/Asset/Asset.h>
#include <Engine/Asset/AssetStreamer.h>
#include <Engine/Asset/AssetSource.h>
#include <Runtime/IO/Path.h>
#include <unordered_set>

namespace Rift::Engine {
	class ENGINE_API AssetSubsystem final : public Subsystem {
	public:
		virtual Runtime::Result OnCreate(Engine* engine) override;
		virtual void OnDestroy() override;
		virtual void GetDependencyGraph(DependencyGraph& graph) override;

		template<class AssetT> requires(std::is_base_of_v<Asset, AssetT>)
			AssetT* Get(AssetHandle handle) {
			if (Asset* existing = TryAsset(handle)) {
				return static_cast<AssetT*>(existing);
			}
			if (AssetInCache(handle)) {
				return static_cast<AssetT*>(Load(handle));
			}
			return nullptr;
		}

		template<class AssetT> requires(std::is_base_of_v<Asset, AssetT>)
		AssetT* Get(const std::string& path, void* extraSettings = nullptr) {
			const std::string assetPath = Runtime::Path::Normalize(path);
			if (assetPath.empty()) {
				Logger::Error("AssetSubsystem", "Empty asset path");
				return nullptr;
			}

			AssetHandle handle;
			handle.index = HashPath(assetPath);

			if (Asset* existing = TryAsset(handle)) return static_cast<AssetT*>(existing);

			if (AssetInCache(handle)) {
				if (Asset* loaded = Load(handle)) return static_cast<AssetT*>(loaded);
				_cachedAssetSources.erase(handle);
			}

			const std::string assetType = AssetT::GetStaticClassName().data();
			AssetStreamer* streamer = GetAssetCookerOfType(assetType);
			if (!streamer) {
				Logger::Error("AssetSubsystem", "No streamer for type '{}'", assetType);
				return nullptr;
			}

			Runtime::Path targetPath = COOKED_ASSET_DIR / Runtime::Path(CookedRelativePath(assetPath)).ReplaceExtension(ASSET_EXTENSION);

			AssetSource source;
			source.handle = handle;
			source.type = assetType;
			source.sourcePath = assetPath;
			source.cookedPath = targetPath.GetPathStr();

			Runtime::Result result = streamer->Cook(source, extraSettings);
			if (!result) {
				Logger::Error("AssetSubsystem", "Failed to cook asset: {}", result.GetMessage());
				return nullptr;
			}

			_cachedAssetSources[handle] = source;
			Asset* loaded = Load(handle);
			if (!loaded) {
				_cachedAssetSources.erase(handle);
				return nullptr;
			}

			SaveRegistry();
			return static_cast<AssetT*>(loaded);
		}

		Asset* TryAsset(AssetHandle handle) {
			if (!_assets.contains(handle)) return nullptr;
			return _assets[handle];
		}

		AssetSource& GetSource(AssetHandle handle) { return _cachedAssetSources[handle]; }

		[[nodiscard]] const std::unordered_map<AssetHandle, Asset*>& GetAssets() const { return _assets; }
		[[nodiscard]] const std::unordered_map<AssetHandle, AssetSource>& GetAssetSources() const { return _cachedAssetSources; }
	private:
		Asset* Load(AssetHandle handle);
		void Unload(AssetHandle handle);

		AssetStreamer* GetAssetCookerOfType(const std::string& type);
		b8 AssetInCache(AssetHandle handle);
		b8 AssetLoaded(AssetHandle handle);
		void SaveRegistry();
		void LoadRegistry();
		
		std::unordered_map<AssetHandle, AssetSource> _cachedAssetSources;

		std::vector<AssetStreamer*> _assetStreamers;
		std::unordered_map<std::string, u32> _assetStreamerLookupTable;

		std::unordered_map<AssetHandle, Asset*> _assets;

		static std::string CookedRelativePath(std::string p) {
			if (p.size() >= 2 && p[1] == ':') p.erase(0, 2);
			while (!p.empty() && p.front() == '/') p.erase(0, 1);
			for (usize pos; (pos = p.find("../")) != std::string::npos; ) p.erase(pos, 3);
			return p;
		}

		static u32 HashPath(std::string_view s) {
			u32 h = 2166136261u;
			for (char c : s) { h ^= static_cast<u32>(std::tolower(static_cast<unsigned char>(c))); h *= 16777619u; }
			return h;
		}
	};
}