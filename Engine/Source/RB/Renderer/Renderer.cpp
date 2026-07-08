#include <pch.h>
#include "RB/Renderer/Renderer.h"
#include "RB/Renderer/Camera.h"
#include "RB/Renderer/Buffers.h"
#include "RB/Renderer/VertexArray.h"
#include "RB/Renderer/Texture.h"
#include "RB/Renderer/Shaders.h"
#include "RB/Renderer/Renderer2D.h"


namespace RB {
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
		RB::RendererCommand::drawIndexed(vertexArray);
	}

}