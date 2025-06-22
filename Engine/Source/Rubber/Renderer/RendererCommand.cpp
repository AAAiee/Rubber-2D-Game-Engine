#include <pch.h>
#include "Rubber/Renderer/RendererCommand.h"
#include "Platform/OpenGL/GLRendererAPI.h"



namespace Rubber {
	// this is mean to live the duration of the entire program, hence left a raw pointer without free it
	//TODO:: Add condition check to intialize s_RendererAPI to be decided during runtime;
	RendererAPI* RendererCommand::s_RendererAPI = new GLRendererAPI();

	void RendererCommand::init(){
		// enable blending
		s_RendererAPI->init();

	}

	void RendererCommand::setViewPort(int width, int height)
	{
		s_RendererAPI->setViewPort(width, height);
	}

	void RendererCommand::clearColor(const glm::vec4& color)
	{
		s_RendererAPI->clearColor(color);
	}

	void RendererCommand::clear()
	{
		s_RendererAPI->clear();
	}

	void RendererCommand::drawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count)
	{
		s_RendererAPI->drawIndexed(vertexArray, count);
	}


}
