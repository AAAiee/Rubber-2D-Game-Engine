#include <pch.h>

#include "Platform/OpenGL/GLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Rubber/Core/Core.h"

Rubber::GLContext::GLContext(GLFWwindow* windowHandler)
	:m_windowHandler(windowHandler)
{
	RB_CORE_ASSERT(this->m_windowHandler, "Context Window Handler is null!");
}

void Rubber::GLContext::init()
{		
	// set the context to the current window
	glfwMakeContextCurrent(this->m_windowHandler);

	// load glad 
	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	RB_CORE_ASSERT(status, "Fail to initialize Glad!");
}

void Rubber::GLContext::swapBuffer()
{
	glfwSwapBuffers(this->m_windowHandler);
}
