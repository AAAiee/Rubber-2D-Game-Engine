#pragma once
#include <glm/glm.hpp>

#define ENABLE_RENDERER_STATS 1


namespace Rubber{
	class Camera;
	class Texture2D;
	class Renderer2D{
		
	public:
		static void init();

		static void shutdown();

		static void beginScene(const Camera& camera);

		static void drawQuad(const glm::vec3& posiiton, const glm::vec2& size, const glm::vec4& color, const float tillingFactor = 1.f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void drawQuad(const glm::vec3& posiiton, const glm::vec2& size, Ref<Texture2D>& tetxure, const float tillingFactor = 1.f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void drawRotatedQuad(const glm::vec3& posiiton, const glm::vec2& size, const float radians, Ref<Texture2D>& tetxure, 
			const float TillingFactor =1.f, const glm::vec4& tintColor = glm::vec4(1.0f));

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
		static void drawTextureQuadHelper(const glm::mat4& transformation,Ref<Texture2D>& texture, const float tillingFacor=1.f, const glm::vec4& tintColor =  glm::vec4(1.0f));  
		static void flush(); 

	private:
#if ENABLE_RENDERER_STATS
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