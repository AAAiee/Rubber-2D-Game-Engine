#include <pch.h>
#include "RB/Renderer/FrameBuffer.h"
#include "RB/Renderer/Renderer.h"
#include "Platform/OpenGL/GLFrameBuffer.h"


namespace RB{


	Ref<FrameBuffer> FrameBuffer::create(const FrameBufferSpecification& spec) 
	{
		switch (Renderer::getAPI())
		{

#ifdef WINDOW_64_API
		case RendererAPI::API::NONE:  RB_CORE_ASSERT(false, "Currently, None-API Mode is not supported!");
		case RendererAPI::API::OpenGL: return makeRef<GLFrameBuffer>(spec);
#endif
		}

		RB_CORE_ASSERT(false, "Currently the renderer API is not supported");
		return nullptr;
	}

}


