#include <glm/glm.hpp>
#include <Rubber/Utility/Utility.h>

namespace Rubber{

	class Texture2D;
	class SubTexture2D{
	public:
		static Ref<SubTexture2D> create(Ref<Texture2D>& SpriteSheet, const glm::vec2 index, const glm::vec2 cellSize, const glm::vec2 spriteSize = { 1, 1 });
		inline const Ref<Texture2D>& getSpriteSheetTexture() const {
			return m_SpriteSheet;
		}

		inline const glm::vec2* getTexCoord() const {
			return m_TexCoord;
		}
 
		SubTexture2D(Ref<Texture2D>& spriteSheet, glm::vec2 minBounds, glm::vec2 maxBounds);
		
	private:
		Ref<Texture2D> m_SpriteSheet;
		glm::vec2  m_TexCoord[4];
	};
}