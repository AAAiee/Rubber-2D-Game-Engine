#include <pch.h>
#include "Rubber/Renderer/Renderer.h"
#include "Rubber/Renderer/Camera.h"
#include "Rubber/Renderer/Buffers.h"
#include "Rubber/Renderer/VertexArray.h"
#include "Rubber/Renderer/Texture.h"
#include "Rubber/Renderer/Shaders.h"
#include "Rubber/Renderer/Renderer2D.h"


namespace Rubber {
	Renderer::SceneData* Renderer::s_SceneDataPtr = new Renderer::SceneData();

	void Renderer::init()
	{
		RendererCommand::init();
		Renderer2D::init();
	}

	void Renderer::onWindowResize(int width, int height)
	{
		RendererCommand::setViewPort(width, height);
	}

	void Renderer::shutdown()
	{
		if (s_SceneDataPtr) {
			delete s_SceneDataPtr;
			s_SceneDataPtr = nullptr;
		}
		Renderer2D::shutdown();
		RendererCommand::shutdown();
	}

	/**
	* responsible for handle all factors relates to environment
	* 
	*/
	void Renderer::beginScene(const Camera& camera)
	{
	}

	void Renderer::endScene()
	{

	}

	void Renderer::submit(const Ref<Texture>& texture, const Ref<Shader>& shader,const Ref<VertexArray>& vertexArray)
	{
		vertexArray->bind();
		shader->bind();
		texture->bind(0);

		shader->setMat4("u_ViewProjectionMatrix", s_SceneDataPtr->m_VPMatrix);
		//those are just very temporary
		shader->setInt("u_texture", 0);
		Rubber::RendererCommand::drawIndexed(vertexArray);
	}

}