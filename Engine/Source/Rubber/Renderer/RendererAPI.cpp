#include <pch.h>
#include "Rubber/Renderer/RendererAPI.h"


namespace Rubber{

	void RendererAPI::init()
	{
		// enable blending here~

	}

	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;
}