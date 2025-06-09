#pragma once
#include "Rubber/Renderer/RendererAPI.h"
#include "Rubber/Core/Core.h"

namespace Rubber {

	class GLRendererAPI : public  RendererAPI {
		// Inherited via RendererAPI
		virtual void clearColor(const glm::vec4& color) final override;
		virtual void setViewPort(int width, int height) override;

		virtual void clear() final override;

		virtual void drawIndexed(const Ref<VertexArray>& vertexArray) final override;
	};
}