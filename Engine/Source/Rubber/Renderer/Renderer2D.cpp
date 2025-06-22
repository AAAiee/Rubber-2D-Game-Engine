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

	struct Vertex{
		glm::vec4 m_Position{};
		glm::vec4 m_Color{};
		glm::vec2 m_TexCoord{};
		float m_TexIndex = 0.0f;
		float m_TillingFactor = 0.0f;
		glm::vec4 m_TintFacor{};
	};

	struct RendererData{
		static constexpr const uint32_t  MAX_QUAD_PER_DRAW = 10000;
		static constexpr const uint32_t  MAX_VERTEX_PER_DRAW = MAX_QUAD_PER_DRAW * 4;
		static constexpr const uint32_t  MAX_INDEX_NUMBER_PER_DRAW = MAX_QUAD_PER_DRAW * 6;
		static constexpr const uint32_t  MAX_TEXTURE_SLOTS = 32; // 32 for now, latter changed for renderer cap
		static constexpr const glm::vec4 UNIT_QUAD_POS[4] = {
			{-0.5f, -0.5f, 0.0f, 1.0f},
			{ 0.5f, -0.5f, 0.0f, 1.0f},
			{ 0.5f,  0.5f, 0.0f, 1.0f},
			{ -0.5f, 0.5f, 0.0f, 1.0f}
		};
		static constexpr const glm::vec2 UNIT_QUAD_TEX_COORD[4] = {
			{0.0f, 0.0f},
			{1.0f, 0.0f},
			{1.0f, 1.0f},
			{0.0f, 1.0f}
		};

		std::array<Ref<Texture>, MAX_TEXTURE_SLOTS> m_TexturesMap;
		uint32_t  m_ValidTextureCount = 0ui32;

		Ref<VertexArray> m_VAO = nullptr;
		Ref<VertexBuffer> m_VBO = nullptr;
		Ref<IndexBuffer> m_IBO = nullptr;
		Ref<ShaderLib> m_ShaderLib = nullptr;
		Ref<Texture2D> m_WhiteTexture = nullptr;

		Vertex* m_Vertices = nullptr;
		Vertex* m_VerticesInsertPosPtr = nullptr;

		uint32_t* m_Indices = nullptr;
		uint32_t m_ValidIndexCount = 0ui32;
	
		~RendererData()
		{
			delete[] m_Vertices;
		}
	};

	RendererData data = RendererData();
#if ENABLE_RENDERER_STATS
	Renderer2D::RendererStats Renderer2D::s_Stats;
#endif 

	void Renderer2D::init()
	{
		BufferLayout VBLayout = {
			{ShaderType::Float4, "aPos"      ,     false},
			{ShaderType::Float4, "aColor"    ,     false},
			{ShaderType::Float2, "aTexCoord" ,     false},
			{ShaderType::Float,  "aTexIndex" ,     false},
			{ShaderType::Float,  "aTillingFactor", false},
			{ShaderType::Float4, "aTintColor",     false }
		};

		data.m_VAO = VertexArray::create();
		data.m_VAO->bind();
		data.m_VBO = VertexBuffer::create(sizeof(Vertex) * data.MAX_VERTEX_PER_DRAW);

		data.m_VBO->setLayout(VBLayout);
		data.m_IBO = IndexBuffer::create(sizeof(uint32_t) * data.MAX_INDEX_NUMBER_PER_DRAW);

		data.m_VAO->addVertexBuffer(data.m_VBO);
		data.m_VAO->setIndexBuffer(data.m_IBO);

		data.m_ShaderLib = makeRef<ShaderLib>();
		data.m_ShaderLib->load("Asset/shader/CommonShader.glsl");

		data.m_WhiteTexture = Texture2D::create(1,1);

		uint32_t whiteTexture = 0xFFFFFFFF;
		data.m_WhiteTexture->setData(&whiteTexture, 4);

		// set up a buffer to store all vertices ..
		data.m_Vertices = new Vertex[RendererData::MAX_VERTEX_PER_DRAW];

		//set up a buffer to store all indices 
		data.m_Indices = new uint32_t[RendererData::MAX_INDEX_NUMBER_PER_DRAW];

		// fill in index for maxQuad
		for (uint32_t i = 0, offset = 0; i + 5 < data.MAX_INDEX_NUMBER_PER_DRAW; i += 6 ) {
			data.m_Indices[i] = offset + 0;
			data.m_Indices[i + 1] = offset + 1;
			data.m_Indices[i + 2] = offset + 2;
			data.m_Indices[i + 3] = offset + 0;
			data.m_Indices[i + 4] = offset + 2;
			data.m_Indices[i + 5] = offset + 3;
			offset += 4;
		}

		data.m_IBO->uploadIndexData(data.m_Indices, data.MAX_INDEX_NUMBER_PER_DRAW * sizeof(uint32_t));
		delete[] data.m_Indices;

		// texture
		data.m_TexturesMap[0] = data.m_WhiteTexture;
		int samplers[data.MAX_TEXTURE_SLOTS];
		for (int i = 0; i < data.MAX_TEXTURE_SLOTS; ++i) {
			samplers[i] = i;
		}
		data.m_ShaderLib->getShader("CommonShader")->setIntArray("u_Textures", samplers, data.MAX_TEXTURE_SLOTS);

		//initialize 
		data.m_ValidTextureCount++;
		data.m_VerticesInsertPosPtr = data.m_Vertices;

#if ENABLE_RENDERER_STATS
		std::memset(&s_Stats, 0ui32, sizeof(RendererStats));
#endif 
	}

	void Renderer2D::shutdown()
	{
		
	}

	void Renderer2D::beginScene(const Camera& camera)
	{
		RB_PROFILE_FUNC();
		Ref<Shader> shader = data.m_ShaderLib->getShader("CommonShader");
		shader->setMat4("u_ViewProjectionMatrix", camera.getViewProjectionMatrix()); //TODO: AN  ORTHOGONAL MATRIX FOR 2D
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color, const float tillingFactor, const glm::vec4& tintColor)
	{
		RB_PROFILE_FUNC();
		//scale->translation

		if (data.m_ValidIndexCount >= data.MAX_INDEX_NUMBER_PER_DRAW){
			flush();
		}
		
		const float texIndex = 0.0f; // in this function we only draw color, no texture(white texture by default)
 
		glm::mat4 tsMatrix = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(scale.x, scale.y, 1.0f)), position);

		for (int i = 0; i < 4; i++){
			data.m_VerticesInsertPosPtr->m_Position = tsMatrix * data.UNIT_QUAD_POS[i];
			data.m_VerticesInsertPosPtr->m_Color = color;
			data.m_VerticesInsertPosPtr->m_TexCoord = data.UNIT_QUAD_TEX_COORD[i];
			data.m_VerticesInsertPosPtr->m_TexIndex = {texIndex};
			data.m_VerticesInsertPosPtr->m_TillingFactor = tillingFactor; 
			data.m_VerticesInsertPosPtr->m_TintFacor = tintColor; 
			data.m_VerticesInsertPosPtr++;
		}
		data.m_ValidIndexCount += 6;
#if ENABLE_RENDERER_STATS 
	s_Stats.m_QuadNumber++;
#endif
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& scale, Ref<Texture2D>& texture, const float tillingFactor, const glm::vec4& tintColor)
	{
		RB_PROFILE_FUNC();

		if (data.m_ValidIndexCount >= data.MAX_INDEX_NUMBER_PER_DRAW) {
			flush();
		}
	
		glm::mat4 tsMatrix = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(scale.x, scale.y, 1.0f)), position);
		drawTextureQuadHelper(tsMatrix, texture, tillingFactor, tintColor);
	}
	

    void Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& scale, const float angles, Ref<Texture2D>& texture, const float tillingFactor, const glm::vec4& tintColor)
    {  
       RB_PROFILE_FUNC();  

	   if (data.m_ValidIndexCount >= data.MAX_INDEX_NUMBER_PER_DRAW) {
		   flush();
	   }

	   glm::mat4 tsMatrix = glm::translate(
		   glm::rotate(
			   glm::scale(glm::mat4(1.0f), glm::vec3(scale.x, scale.y, 1.0f)),
			   glm::radians(angles), glm::vec3(0.0f, 0.0f, 1.0f)) ,position);

	   drawTextureQuadHelper(tsMatrix, texture, tillingFactor, tintColor);
    }

	void Renderer2D::endScene()
	{
		flush();
	}

	void Renderer2D::flush()
	{
		RB_PROFILE_FUNC();
		data.m_VBO->uploadVertexData(data.m_Vertices, static_cast<uint32_t>((data.m_VerticesInsertPosPtr - data.m_Vertices)) * static_cast<uint32_t> (sizeof(Vertex)));
		for(uint32_t i = 0;  i < data.m_ValidTextureCount ; ++i){
			data.m_TexturesMap[i]->bind(i);
		}
		RendererCommand::drawIndexed(data.m_VAO, data.m_ValidIndexCount);

		// clean up for next batch
		data.m_VerticesInsertPosPtr = data.m_Vertices;
		data.m_ValidIndexCount = 0;
		data.m_ValidTextureCount = 1;
#if ENABLE_RENDERER_STATS 
	s_Stats.m_DrawCallCount++;
#endif
	}


	void Renderer2D::drawTextureQuadHelper(const glm::mat4& transformation, Ref<Texture2D>& texture, const float tillingFactor, const glm::vec4& tintColor)
	{
		RB_PROFILE_FUNC();
		//scale->translation
		float texIndex = 0.0f;

		// check if we already have this texture, if we do, just use that as the index
		for (uint32_t i = 1; i < data.m_ValidTextureCount; i++) {
			if (*texture == *(data.m_TexturesMap[i])) {
				texIndex = static_cast<float>(i);
				break;
			}
		}

		if (texIndex == 0.0f) { // new texture has been passed in, store it in a new texture slot
			RB_CORE_ASSERT(data.m_ValidTextureCount < data.MAX_TEXTURE_SLOTS, "Cannot have more new textures, slot run out!");
			data.m_TexturesMap[data.m_ValidTextureCount] = texture;
			texIndex = static_cast<float> (data.m_ValidTextureCount);
			data.m_ValidTextureCount++;
		}

		RB_CORE_ASSERT(texIndex != 0.0f, "Texture is not binded sucessfully!");

		const glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f }; //white for default, since we draw texture instead of a color

		for (int i = 0; i < 4; i++) {
			data.m_VerticesInsertPosPtr->m_Position = transformation * RendererData::UNIT_QUAD_POS[i];
			data.m_VerticesInsertPosPtr->m_Color = color;
			data.m_VerticesInsertPosPtr->m_TexCoord = RendererData::UNIT_QUAD_TEX_COORD[i];
			data.m_VerticesInsertPosPtr->m_TexIndex = { texIndex };
			data.m_VerticesInsertPosPtr->m_TillingFactor = tillingFactor;
			data.m_VerticesInsertPosPtr->m_TintFacor = tintColor;
			data.m_VerticesInsertPosPtr++;
		}
		data.m_ValidIndexCount += 6;

#if ENABLE_RENDERER_STATS
		s_Stats.m_QuadNumber++;
#endif 
	}


#if ENABLE_RENDERER_STATS
	
	uint32_t Renderer2D::getDrawCallCount()
	{
		return s_Stats.m_DrawCallCount;
	}

	uint32_t Renderer2D::getQuadCount()
	{
		return s_Stats.m_QuadNumber;

	}

	uint32_t Renderer2D::getVertexCount()
	{
		return s_Stats.getVertexCount();

	}

	uint32_t Renderer2D::getIndexCount()
	{
		return s_Stats.getIndexCount();
	}

	void Renderer2D::resetRendererStat()
	{
		std::memset(&s_Stats, 0ui32, sizeof(RendererStats));
	}

#endif

}
