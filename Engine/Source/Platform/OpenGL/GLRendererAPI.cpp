#include "pch.h"
#include <glad/glad.h>
#include "Rubber/Renderer/VertexArray.h"
#include "Rubber/Renderer/Buffers.h"
#include "GLRendererAPI.h"

void Rubber::GLRendererAPI::init()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

}

void Rubber::GLRendererAPI::clearColor(const glm::vec4& color)
{
	glClearColor(color.x, color.y, color.z, color.w);
}

void Rubber::GLRendererAPI::setViewPort(int width, int height)
{
	glViewport(0, 0, width, height);

}

void Rubber::GLRendererAPI::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Rubber::GLRendererAPI::drawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count )
{
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr); 
}
