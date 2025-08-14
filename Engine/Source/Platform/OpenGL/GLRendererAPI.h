#pragma once
#include "Rubber/Renderer/RendererAPI.h"
#include "Rubber/Core/Core.h"

namespace Rubber {

	class GLRendererAPI : public  RendererAPI {
		// Inherited via RendererAPI
		virtual void init()final  override;
		virtual void clearColor(const glm::vec4& color) final override;
		virtual void setViewPort(int width, int height) override;
		virtual void setPolygonMode(RendererPolygonMode mode) override;

		virtual void clear() final override;

		virtual void drawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count=0) final override;
	};
}