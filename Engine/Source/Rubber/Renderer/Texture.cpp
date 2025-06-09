#include <pch.h>
#include "Rubber/Renderer/Texture.h"
#include "Rubber/Renderer/Renderer.h"

#include "Platform/OpenGL/GLTexture2D.h"
namespace Rubber{
	Ref<Texture2D> Texture2D::create(const std::string& texturePath)
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

	Ref<Rubber::Texture2D> Texture2D::create(uint16_t width, uint16_t height)
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
