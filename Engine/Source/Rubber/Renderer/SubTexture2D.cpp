#include <pch.h>
#include "Subtexture2D.h"
#include "Rubber/Renderer/Texture.h"

namespace Rubber{


	Ref<SubTexture2D> SubTexture2D::creatFromIndex(const Ref<Texture2D>& SpriteSheet, const glm::vec2 index, const glm::vec2 cellSize, const glm::vec2 spriteSize /*= { 1, 1 }*/, bool oppositeDireciton)
	{
		float SheetWidth = static_cast<float> (SpriteSheet->getWidth());
		float SheetHeight = static_cast <float> (SpriteSheet->getHeight());
	
		glm::vec2 leftBottom;
		glm::vec2 rightTop;
		if (!oppositeDireciton) {
			leftBottom = { index.x * cellSize.x / SheetWidth , index.y * cellSize.y / SheetHeight };
			rightTop = { (index.x + spriteSize.x) * cellSize.x / SheetWidth, (index.y + spriteSize.y) * cellSize.y / SheetHeight };
		}
		else {
			leftBottom = { (index.x + spriteSize.x) * cellSize.x / SheetWidth, index.y * cellSize.y / SheetHeight };
			rightTop = { index.x * cellSize.x / SheetWidth ,(index.y + spriteSize.y) * cellSize.y / SheetHeight };
		}

		return makeRef<SubTexture2D>(SpriteSheet,leftBottom, rightTop);
	}


	Ref<SubTexture2D> SubTexture2D::createFromTexture(const Ref<Texture2D>& spriteSheet, bool isOppositeDireciton)
	{
		if (!isOppositeDireciton) {
			return makeRef<SubTexture2D>(spriteSheet, glm::vec2{ 0,0 }, glm::vec2{ 1,1 });
		}
		return makeRef<SubTexture2D>(spriteSheet, glm::vec2{ 1,0 }, glm::vec2{ 0,1 });
	}

	Ref<SubTexture2D> SubTexture2D::create()
	{
		return makeRef<SubTexture2D>();
	}

	SubTexture2D::SubTexture2D(const Ref<Texture2D>& spriteSheet, glm::vec2 leftBottom, glm::vec2 rightTop)
		:m_SpriteSheet(spriteSheet)
	{
		m_TexCoord[0] = { leftBottom.x, leftBottom.y }; // 0 , 0 
		m_TexCoord[1] = { rightTop.x, leftBottom.y };   // 1 , 0
		m_TexCoord[2] = { rightTop.x, rightTop.y };     // 1 , 1
		m_TexCoord[3] = { leftBottom.x, rightTop.y };   // 0 , 1 

		m_Dimension.x = glm::abs(rightTop.x - leftBottom.x) * spriteSheet->getWidth();
		m_Dimension.y = glm::abs(rightTop.y - leftBottom.y) * spriteSheet->getHeight();
	}
}


