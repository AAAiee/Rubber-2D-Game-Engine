#include <pch.h>
#include "Buffers.h"
#include "Rubber/Core/Core.h"
#include "Rubber/Renderer/Renderer.h"

#include "Rubber/Renderer/RendererAPI.h"
#include "Platform/OpenGL/GLBuffers.h"


namespace Rubber {

	Scope<VertexBuffer> Rubber::VertexBuffer::create(float* vertices, size_t size)
	{
		switch (Renderer::getAPI())
		{

#ifdef WINDOW_64_API
		case RendererAPI::API::NONE:  RB_CORE_ASSERT(false, "Currently, None-API Mode is not supported!");
		case RendererAPI::API::OpenGL: return makeScope<GLVertexBuffer> (vertices, size);
#endif
		}

		RB_CORE_ASSERT(false, "Currently the renderer API is not supported");
		return nullptr;
	}


	Scope<IndexBuffer> IndexBuffer::create(uint32_t* indices, size_t size)
	{
		switch (Renderer::getAPI())
		{

#ifdef WINDOW_64_API
		case RendererAPI::API::NONE:  RB_CORE_ASSERT(false, "Currently, None-API Mode is not supported!");
		case RendererAPI::API::OpenGL: return makeScope<GLIndexBuffer>(indices, size);
#endif
		}

		RB_CORE_ASSERT(false, "Currently the renderer API is not supported");
		return nullptr;
	}

}


