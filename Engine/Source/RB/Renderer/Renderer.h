#pragma once
#include "RB/Renderer/RendererCommand.h"

/**
* @file Renderer.h
* @brief Renderer.h is a header file that contains the definition of the Renderer class.
*/
namespace RB {

	class Camera;
	class Shader;
	class Texture;
	class Renderer {
	public:

		static void init();
		static void onWindowResize(int width, int height);
		static void beginScene(const Camera& camera);
		static void endScene();
		static void submit(const Ref<Texture>& texture,const Ref<Shader>& shader,const Ref<VertexArray>& vertexArray);
		static inline RendererAPI::API getAPI() { return RendererAPI::getAPI();}


	private:
		struct SceneData {
			glm::mat4 m_VPMatrix;

			SceneData()
				: m_VPMatrix(glm::mat4(1.0f))
			{
			}
		};

		static SceneData* s_SceneDataPtr;
	};

}