#pragma once
#include "Rubber/Renderer/GraphicsContext.h"

struct GLFWwindow;
namespace Rubber {

	class GLContext : public GraphicsContext
	{
	public:
		GLContext(GLFWwindow* windowHandler);
		~GLContext() = default;
		virtual void init() final override;
		virtual void swapBuffer() final override;

	private:
		GLFWwindow* m_windowHandler;
	};
}
