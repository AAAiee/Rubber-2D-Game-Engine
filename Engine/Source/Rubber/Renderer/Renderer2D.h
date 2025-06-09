#pragma once
#include <glm/glm.hpp>


namespace Rubber{
	class Camera;
	class Renderer2D{
		
	public:
		static void init();

		static void shutDown();

		static void beginScene(const Camera& camera);

		static void drawQuad(const glm::vec3& posiiton, const glm::vec3& size, const glm::vec4& color);

		static void endScene();

	};


}