#include <pch.h>
#include "Rubber/Resources/AssetManager.h"
#include "Rubber/Resources/AssetImporter.h"

namespace Rubber {



	bool AssetManager::isAssetLoaded(AssetHandle handle) const
	{
		if (m_AssetRegistry.find(handle) != m_AssetRegistry.end()) {
			return  true;
		}
		return false;
	}

	Ref<Asset>& AssetManager::getAsset(AssetHandle handle)
	{
		RB_CORE_ASSERT(isAssetLoaded(handle), "The asset is not loaded!");
		return m_AssetMap[handle];
	}

    const Ref<Asset>& AssetManager::getAsset(AssetHandle handle) const  
    {  
       RB_CORE_ASSERT(isAssetLoaded(handle), "The asset is not loaded!");  
	   return  m_AssetMap.at(handle);
    }

	const AssetMetaData& AssetManager::getAssetMetadata(AssetHandle handle) const
	{
		RB_CORE_ASSERT(isAssetLoaded(handle), "can't find the asset!");
		return *m_AssetRegistry.at(handle);
	}

}