#pragma  once
#include <glm/glm.hpp>
#include <RB/Utility/Utility.h>

namespace RB{

	class Texture2D;
	class SubTexture2D {
	public:
		inline const Ref<Texture2D>& getSpriteSheetTexture()  {
			return m_SpriteSheet;
		}
		inline const glm::vec2* getTexCoord() const {
			return m_TexCoord;
		}

		SubTexture2D(const Ref<Texture2D>& spriteSheet, glm::vec2 minBounds, glm::vec2 maxBounds);
		SubTexture2D() = default;
		~SubTexture2D() = default;

		inline glm::vec2 getSubtextureDimension() const {
			return m_Dimension;
		}


	public:
		static Ref<SubTexture2D> creatFromIndex(const Ref<Texture2D>& SpriteSheet, const glm::vec2 index, const glm::vec2 cellSize, const glm::vec2 spriteSize = { 1, 1 }, bool oppositeDirection = false);

		static Ref<SubTexture2D> createFromTexture(const Ref<Texture2D>& spriteSheet, bool isOppositeDireciton);

		static Ref<SubTexture2D> create();

		
		
	private:
		Ref<Texture2D>  m_SpriteSheet;
		glm::vec2 m_TexCoord[4]{};;
		glm::vec2 m_Dimension{};
	};
}