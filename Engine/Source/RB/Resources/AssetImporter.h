#pragma  once

namespace RB {
	struct AssetMetaData;
	class AssetManager;
	class Asset;

	class AssetImporter {
	public:
		static Ref<Asset> importAsset(AssetManager& manager, const AssetMetaData& metaData);
	};
}