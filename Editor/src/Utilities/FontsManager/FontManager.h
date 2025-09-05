#pragma once

#include <imgui.h>
#include <string_view>
#include <string>
#include <unordered_map>
#include <filesystem>

struct FontInfo {
	std::string name;
	std::filesystem::path path;
	float size;
};


class FontManager {
public:

	static void loadFontFromTTF(FontInfo info);

	static ImFont* getFont(std::string_view fontName);

	static void setDefaultFont(std::string_view fontName);
};


