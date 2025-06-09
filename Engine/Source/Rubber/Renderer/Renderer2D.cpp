#include <pch.h>
#include "Renderer2D.h"
#include "Rubber/Renderer/BufferLayout.h"
#include "Rubber/Renderer/Shaders.h"
#include "Rubber/Renderer/Buffers.h"
#include "Rubber/Renderer/VertexArray.h"
#include "Rubber/Renderer/Texture.h"
#include "Rubber/Renderer/Camera.h"
#include "Rubber/Renderer/RendererCommand.h"

namespace Rubber {

	static struct RendererData{
		Ref<VertexArray> vao;
		Ref<ShaderLib> shaderLib;
		Ref<Texture2D> whiteTexture;
	};

	auto data = new Rubber::RendererData();

	void Renderer2D::init()
	{
		
		// vertex Buffer layout
		BufferLayout layout = {
			{ShaderType::Float3, "aPos", false },
			{ShaderType::Float2, "aTexCoord", false}
		};

		/////////////////////////////////Draw a square below the triangle
		float blueSqVertices[4 * 5] = {
			-0.5f  , -0.5f  , 0.0f,  0.0, 0.0,  // left bottom
			 0.5f  , -0.5f  , 0.0f,  1.0, 0.0,  // right bottom 
			-0.5f  ,  0.5f  , 0.0f,  0.0, 1.0,  // left top
			 0.5f  ,  0.5f  , 0.0f,  1.0, 1.0   // right top
		};

		uint32_t blueSquareIndices[3 * 2] = {
			0 , 1, 2, 2, 3, 1
		};

		data->vao = VertexArray::create();
		data->vao->bind();
		Ref<VertexBuffer> blueSquareVertexBuffer = VertexBuffer::create(blueSqVertices, sizeof(blueSqVertices));

		blueSquareVertexBuffer->setLayout(layout);
		Ref<IndexBuffer> blueSquareIndexBuffer = IndexBuffer::create(blueSquareIndices, sizeof(blueSquareIndices));

		data->vao->addVertexBuffer(blueSquareVertexBuffer);
		data->vao->setIndexBuffer(blueSquareIndexBuffer);

		data->shaderLib = makeRef<ShaderLib>();
		data->shaderLib->load("Asset/shader/CommonShader.glsl");

		data->whiteTexture = Texture2D::create(1,1);
		data->whiteTexture->bind(0);
		uint32_t whiteTexture = 0xFFFFFFFF;
		data->whiteTexture->setData(&whiteTexture, 4);

	}

	void Renderer2D::beginScene(const Camera& camera)
	{
		Ref<Shader> shader = data->shaderLib->getShader("CommonShader");
		shader->setMat4("u_ViewProjectionMatrix", camera.getViewProjectionMatrix()) ;
		shader->setInt("u_texture", 0);

	}

	void Renderer2D::drawQuad(const glm::vec3& posiiton, const glm::vec3& size, const glm::vec4& color)
	{
		data->whiteTexture->bind(0);
		glm::mat4 modelMatrix = glm::scale(glm::translate(glm::mat4(1.0f), posiiton), size);
		Ref<Shader> shader = data->shaderLib->getShader("CommonShader");
		shader->setMat4("u_ModelMatrix", modelMatrix);
		shader->setFloat4("u_Color",  color);

		RendererCommand::drawIndexed(data->vao);
	}

	void Renderer2D::endScene()
	{

	}
}
