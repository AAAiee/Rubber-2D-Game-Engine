#pragma once
#include <glm/glm.hpp>

#define ENABLE_RENDERER_STATS 1


namespace Rubber{
	class Camera;
	class Texture2D;
	class SubTexture2D;
	class OrthoCamera;


	class Renderer2D{
		
	public:
		static void init();

		static void shutdown();

		static void beginScene(const glm::mat4& vpMatrix);

		//legacy, to delete 
		static void beginScene(const OrthoCamera& camera);

		// Direct Input, Texture
		static void drawQuad(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color, float tillingFactor = 1.f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void drawQuad(const glm::vec3& position, const glm::vec2& scale, const Ref<Texture2D>& texture, float tillingFactor = 1.f, const glm::vec4& tintColor = glm::vec4(1.0f));

		//Direct Input, SubTexture, non-Rotated quad
		static void drawQuad(const glm::vec3& position, const glm::vec2& scale, const Ref<SubTexture2D>& subTexture, const float tillingFactor = 1.f, const glm::vec4& tintColor = glm::vec4(1.0f));

		// Direct Input, Rotated, texture
		static void drawRotatedQuad(const glm::vec3& position, const glm::vec2& scale,  float radians, const Ref<Texture2D>& texture, 
			const float TillingFactor =1.f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void drawRotatedQuad(const glm::vec3& position, const glm::vec2& scale,  float radians, const glm::vec4& color,
			const float TillingFactor = 1.f, const glm::vec4& tintColor = glm::vec4(1.0f));

		// Direct Input, Rotated, SubTexture Overload
		static void drawRotatedQuad(const glm::vec3& position, const glm::vec2& scale,  float radians, const Ref<SubTexture2D>& subTexture,
			const float TillingFactor = 1.f, const glm::vec4& tintColor = glm::vec4(1.0f));

		//Matrix Overload
		static void drawTextureQuad(const glm::mat4& transformation, const Ref<Texture2D>& texture, const glm::vec2* texCoord=nullptr, float tillingFacor = 1.f, const glm::vec4& tintColor = glm::vec4(1.0f));

		static void drawColorQuad(const glm::mat4& transformation, const glm::vec4& color,  float tillingFacor = 1.f, const glm::vec4& tintColor = glm::vec4(1.0f));


		static void endScene();

#if ENABLE_RENDERER_STATS
		//render stats
		static uint32_t getDrawCallCount(); 
		static uint32_t getQuadCount();
		static uint32_t getVertexCount();
		static uint32_t getIndexCount();
		static void resetRendererStat();
#endif

	private:
		static void flush(); 


#if ENABLE_RENDERER_STATS
	private:
		struct RendererStats{
			uint32_t m_QuadNumber;
			uint32_t m_DrawCallCount;
			
			inline uint32_t getVertexCount(){
				return m_QuadNumber * 4;
			} 

			inline uint32_t getIndexCount(){
				return m_QuadNumber * 6;
			} 
		};


	public:
		static RendererStats s_Stats;
#endif 
	};


}