#include <Rubber.h>
#include "FontManager.h"
#include <sstream>


static std::unordered_map < std::string, uint32_t, RB::stringHash, std::equal_to<>> s_FontsMap;
static uint32_t s_FontCount = 0;

void FontManager::loadFontFromTTF(FontInfo fontInfo)
{
	auto& io = ImGui::GetIO();

	io.Fonts->AddFontFromFileTTF(fontInfo.path.string().c_str(), fontInfo.size);

	s_FontsMap[fontInfo.name] = s_FontCount;
	s_FontCount++;
}

ImFont* FontManager::getFont(std::string_view fontName)
{
	RB_ASSERT(s_FontsMap.find(fontName) != s_FontsMap.end());

	uint32_t pos = s_FontsMap[fontName.data()];
	return ImGui::GetIO().Fonts->Fonts[pos];
}

void FontManager::setDefaultFont(std::string_view fontName)
{
	ImGui::GetIO().FontDefault = getFont(fontName);
}

