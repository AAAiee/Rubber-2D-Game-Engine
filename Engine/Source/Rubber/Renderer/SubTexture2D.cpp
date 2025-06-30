#include <pch.h>
#include "Subtexture2D.h"
#include "Rubber/Renderer/Texture.h"

namespace Rubber{

	Ref<SubTexture2D> SubTexture2D::create(Ref<Texture2D>& SpriteSheet, const glm::vec2 index, const glm::vec2 cellSize, const glm::vec2 spriteSize /*= { 1, 1 }*/)
	{
		float SheetWidth = static_cast<float> (SpriteSheet->getWidth());
		float SheetHeight = static_cast <float> (SpriteSheet->getHeight());

		glm::vec2 leftBottom = {  index.x * cellSize.x / SheetWidth , index.y * cellSize.y / SheetHeight};
		glm::vec2 rightTop   = { (index.x + spriteSize.x) * cellSize.x / SheetWidth, (index.y + spriteSize.y) * cellSize.y / SheetHeight };

		return makeRef<SubTexture2D>(SpriteSheet,leftBottom,  rightTop);
	}

	SubTexture2D::SubTexture2D(Ref<Texture2D>& spriteSheet, glm::vec2 leftBottom, glm::vec2 rightTop)
		:m_SpriteSheet(spriteSheet)
	{
		// minBounds = {0, 0 } left bottom, maxBounds  = {1,1}
		m_TexCoord[0] = { leftBottom.x, leftBottom.y }; // 0 , 0 
		m_TexCoord[1] = { rightTop.x, leftBottom.y };   // 1 , 0
		m_TexCoord[2] = { rightTop.x, rightTop.y };     // 1 , 1
		m_TexCoord[3] = { leftBottom.x, rightTop.y };   // 0 , 1 
	}
}


