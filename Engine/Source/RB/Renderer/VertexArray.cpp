#include "pch.h"
#include "VertexArray.h"
#include "RB/Renderer/Renderer.h"
#include "RB/Renderer/Buffers.h"
#include "RB/Renderer/RendererAPI.h"
#include "Platform/OpenGl/GLVertexArray.h"

namespace RB {
	Ref<VertexArray> RB::VertexArray::create()
    {
		switch (Renderer::getAPI())
		{

#ifdef WINDOW_64_API
		case RendererAPI::API::NONE:  RB_CORE_ASSERT(false, "Currently, None-API Mode is not supported!");
		case RendererAPI::API::OpenGL: return makeRef<GLVertexArray>();
#endif
		}

		RB_CORE_ASSERT(false, "Currently the renderer API is not supported");
		return nullptr;
    }
}
