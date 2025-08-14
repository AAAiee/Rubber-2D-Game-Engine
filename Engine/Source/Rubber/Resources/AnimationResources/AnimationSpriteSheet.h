#pragma once
#include <cstdint>
#include <string>
#include "Rubber/Resources/Asset.h"


namespace Rubber {
	class Texture2D;
	class SubTexture2D;
	class AssetManager;

	class AnimationSpriteSheet: public Asset {
	public:
		AnimationSpriteSheet() = default;
		~AnimationSpriteSheet() = default;

		AnimationSpriteSheet(AssetHandle handle,  uint32_t frameCount,  AssetManager* manager);

		static AssetType s_GetType();
	    AssetType getType() const override;

		inline void setManager(AssetManager* manager) {
			m_Manager = manager;
		}
	    Ref<SubTexture2D> getSubTextureAt(uint32_t col, uint32_t row=0ui32,  bool oppositeDirection= false);
		
	private:
		//USINg weakRef ?
		AssetManager* m_Manager = nullptr;
		uint32_t m_FrameCount = 0;
		AssetHandle m_AssetHandle;
	};
}