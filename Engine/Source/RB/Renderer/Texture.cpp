#include <pch.h>
#include "RB/Renderer/Texture.h"
#include "RB/Renderer/Renderer.h"

#include "Platform/OpenGL/GLTexture2D.h"
namespace RB{
	Ref<Texture2D> Texture2D::create(std::string_view texturePath)
	{
		switch (Renderer::getAPI())
		{

#ifdef WINDOW_64_API
		case RendererAPI::API::NONE:  RB_CORE_ASSERT(false, "Currently, None-API Mode is not supported!");
		case RendererAPI::API::OpenGL: return makeRef<GLTexture2D>(texturePath);
#endif
		}
		RB_CORE_ASSERT(false, "Currently the renderer API is not supported");
		return nullptr;
	}

	Ref<RB::Texture2D> Texture2D::create(uint16_t width, uint16_t height)
	{
		switch (Renderer::getAPI())
		{

#ifdef WINDOW_64_API
		case RendererAPI::API::NONE:  RB_CORE_ASSERT(false, "Currently, None-API Mode is not supported!");
		case RendererAPI::API::OpenGL: return makeRef<GLTexture2D>(width,height);
#endif
		}
		RB_CORE_ASSERT(false, "Currently the renderer API is not supported");
		return nullptr;
	}


}
