#include <pch.h>

#include "Platform/OpenGL/GLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>


RB::GLContext::GLContext(GLFWwindow* windowHandler)
	:m_windowHandler(windowHandler)
{
	RB_CORE_ASSERT(this->m_windowHandler, "Context Window Handler is null!");
}

void RB::GLContext::init()
{		
	RB_PROFILE_FUNC();

	// set the context to the current window
	glfwMakeContextCurrent(this->m_windowHandler);

	// load glad 
	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	RB_CORE_ASSERT(status, "Fail to initialize Glad!");
}

void RB::GLContext::swapBuffer()
{
	RB_PROFILE_FUNC();

	glfwSwapBuffers(this->m_windowHandler);
}
