#pragma once
#include <unordered_map>
#include <Rubber/Resources/Asset.h>
#include "Rubber/Resources/AssetMetaData.h"
#include "Rubber/Resources/AssetImporter.h"

namespace Rubber {
	using  AssetMap = std::unordered_map<AssetHandle, Ref<Asset>>;
	using  AssetRegistry = std::unordered_map<AssetHandle, Scope<AssetMetaData>>;

	class AssetManager
	{
	public:

		AssetManager() = default;
		~AssetManager() = default;

		template<typename T>
		requires std::is_base_of_v<AssetMetaData, T>
		AssetHandle loadAsset(const T& metadata) {
			Ref<Asset> asset = AssetImporter::importAsset(*this, metadata);
			addAsset(metadata.name, metadata, asset);
			return  AssetHandle{ metadata.name };
		}

		template <typename T>
		requires std::is_base_of_v<AssetMetaData, T>
		void addAsset(std::string_view name, const T& metadata, const Ref<Asset>& asset) {
			RB_CORE_ASSERT(!isAssetLoaded(name), "Asset is already loaded!");
			m_AssetMap.emplace(name, asset);
			m_AssetRegistry.emplace(name, makeScope<T>(metadata));
		}

		bool isAssetLoaded(AssetHandle handle) const ;

		Ref<Asset>& getAsset(AssetHandle handle) ;
		const Ref<Asset>& getAsset(AssetHandle handle) const;

		const AssetMetaData& getAssetMetadata( AssetHandle assetHanlde) const ;

	private:
	    AssetMap m_AssetMap;
		AssetRegistry m_AssetRegistry;
	};

}