#pragma once
#include "Rubber/Renderer/RendererAPI.h"
#include "Rubber/Core/Core.h"

namespace Rubber {

	// mid layer between redender and actually renderer API
	class RendererCommand {
	public:
		static void init();
		static void setViewPort(int width, int height);
		static void clearColor(const glm::vec4& color);
		static void clear();
		static void drawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count=0);
	private:
		static RendererAPI* s_RendererAPI;
	};


}
