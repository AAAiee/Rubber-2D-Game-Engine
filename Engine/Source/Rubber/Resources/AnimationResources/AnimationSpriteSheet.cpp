#include <pch.h>
#include "AnimationSpriteSheet.h"
#include "Rubber/Renderer/Texture.h"
#include "Rubber/Renderer/Subtexture2D.h"
#include "Rubber/Resources/AssetManager.h"


namespace Rubber {


	AnimationSpriteSheet::AnimationSpriteSheet(AssetHandle handle, uint32_t frameCount,  AssetManager* manager )
		:m_AssetHandle(handle),m_FrameCount(frameCount), m_Manager(manager)
	{ 
	}

	Rubber::AssetType AnimationSpriteSheet::s_GetType()
	{
		return AssetType::AnimationSpriteSheet;
	}

	Rubber::AssetType AnimationSpriteSheet::getType() const
	{
		return s_GetType();
	}

	Ref<SubTexture2D> AnimationSpriteSheet::getSubTextureAt( uint32_t col,  uint32_t row/*=0*/, bool oppositeDirection/*= false*/)
	{
		RB_CORE_ASSERT(m_Manager != nullptr, "set manager before trying to get texture!");
		const Ref<Asset>& asset = m_Manager->getAsset(m_AssetHandle);
		RB_CORE_ASSERT(asset->getType() == AssetType::Texture2D, "wrong type !");
		const Ref<Texture2D>& texture = static_pointer_cast<Texture2D> (asset);

		uint32_t height = texture->getHeight();
		uint32_t width = texture->getWidth();

		float CellSizeHeight = (float)height;
		float CellSizeWidth = (float)width / (float)m_FrameCount;

		return SubTexture2D::creatFromIndex(texture, {col, row }, { CellSizeWidth, CellSizeHeight }, { 1, 1 }, oppositeDirection);
	}
}

