#pragma  once
#include <Rubber/Utility/Utility.h>
#include "Rubber/Resources/Asset.h"
#include <filesystem>
// A array of texture from separate image file that compose a single animation 
// suppose our asset manager loads all texture 2d we need, and we get a unique identifier to a series of textures together
// form a animation

// actually use a std::array is enough
namespace Rubber {
	class Texture2D;
	class SubTexture2D;
	class AssetManager;

	class AnimationAtlas final : public Asset{
	public:
		AnimationAtlas() = default;
		~AnimationAtlas() = default;

		static AssetType s_GetAssetType() {
			return AssetType::AnimationAtlas;
		}
		virtual AssetType getType() const override {
			return s_GetAssetType();
		}

		void addFrame(AssetHandle& handler);
		void setManager(AssetManager* manager) {
			m_Manager = manager;
		}

		const Ref<Texture2D> getTextureAtIndex(uint32_t index) const;  

	private:
		AssetManager* m_Manager = nullptr;
		Vector<AssetHandle> m_Frames; 
	};
}