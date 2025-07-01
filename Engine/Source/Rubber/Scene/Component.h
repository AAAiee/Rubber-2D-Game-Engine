#pragma once
#include <glm/glm.hpp>
#include <string>
#include <Rubber/Renderer/Camera.h>

namespace Rubber{

	struct TransformComponent{
		glm::mat4 transform;

		TransformComponent(const glm::mat4& tsMatrix)
			:transform(tsMatrix){} 
		~TransformComponent() = default;
		TransformComponent(const TransformComponent& other) = default;
	};


	struct TagComponent{
		std::string tag;

		TagComponent(std::string_view tagName)
			:tag(tagName) {
		}
		~TagComponent() = default;
		TagComponent(const TagComponent& other) = default;
	}; 

	struct SpriteComponent{
		glm::vec4 color;

		SpriteComponent(const glm::vec4& color)
			:color(color){
		}
		~SpriteComponent() = default;
		SpriteComponent(const SpriteComponent& other) = default;
	};


	struct  CameraComponent{
		Camera camera;
	};
	



}