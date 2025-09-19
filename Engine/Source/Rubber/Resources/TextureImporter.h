#pragma once  
#include "Rubber/Core/Utility.h"  

namespace Rubber {  

	class AnimationSpriteSheet;
	struct AssetMetaData;
	class AssetManager;
	class Texture2D; 
	class AnimationAtlas;

	class TextureImporter {  
	public:

		static Ref<Texture2D> importTexture2D(AssetManager& manager, const AssetMetaData& metadata);  

		static Ref<AnimationAtlas> loadTAnimationAtlas(AssetManager& manager,const AssetMetaData& metadata);  

		static Ref<AnimationSpriteSheet> loadAnimationSpriteSheet(AssetManager& manager,const AssetMetaData& metadata);  
		
		static Ref<Texture2D> s_MissingTexture;
	};  
}