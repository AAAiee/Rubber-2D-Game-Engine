#pragma once
#include "RB/Renderer/GraphicsContext.h"

struct GLFWwindow;
namespace RB {

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
