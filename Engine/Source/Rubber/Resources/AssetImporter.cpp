#include <pch.h>

#include "Rubber/Resources/AssetImporter.h"
#include "Rubber/Resources/Asset.h"
#include "Rubber/Resources/AssetMetaData.h"
#include "Rubber/Resources/AssetManager.h"
#include "Rubber/Resources/AnimationResources/AnimationAtlas.h"
#include "Rubber/Resources/AnimationResources/AnimationSpriteSheet.h"
#include "Rubber/Renderer/Texture.h"
#include "Rubber/Resources/TextureImporter.h"

namespace Rubber{

	using AssetImportFunc = std::function<Ref<Asset>(AssetManager&,const AssetMetaData&)>;

	std::unordered_map <AssetType, AssetImportFunc> s_ImportFuncMap = {
		{AssetType::Texture2D,  TextureImporter::importTexture2D },
		{AssetType::AnimationAtlas, TextureImporter::loadTAnimationAtlas},
		{AssetType::AnimationSpriteSheet, TextureImporter::loadAnimationSpriteSheet}
	};


	Ref<Asset> AssetImporter::importAsset(AssetManager& manager, const AssetMetaData& metaData)
	{
		RB_CORE_ASSERT(s_ImportFuncMap.find(metaData.type) != s_ImportFuncMap.end(), "Not supported type!");
		return s_ImportFuncMap.at(metaData.type)(manager, metaData);
	}

}