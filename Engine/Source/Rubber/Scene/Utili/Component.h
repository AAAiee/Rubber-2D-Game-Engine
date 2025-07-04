#pragma once
#include <glm/glm.hpp>
#include <string>
#include <Rubber/Renderer/Camera.h>
#include <Rubber/Scene/SceneCamera.h>
#include <Rubber/Scene/Utili/ScriptableEntity.h>

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


	struct CameraComponent{
		SceneCamera camera;
		
		CameraComponent(const SceneCamera& camera )
			:camera(camera) {
		}

		CameraComponent() = default;
		~CameraComponent() = default;
		CameraComponent(const CameraComponent& other) = default;
	};


	struct PrimaryCameraTag{
		uint8_t dummy = 0;

	};


	struct NativeScriptComponent{  // TODO : Considering using raw function pointers? will it be better?
		using instantiateScriptFunc = ScriptableEntity*(*)();
		using destroyScriptFunc     = void(*)(NativeScriptComponent*);

		ScriptableEntity* instance = nullptr;
		instantiateScriptFunc initScript = nullptr;
		destroyScriptFunc     destroyScript = nullptr;

		template <typename T>
		void bind(){
			initScript =    []() { return static_cast<ScriptableEntity*>(new T()); };
			destroyScript = [](NativeScriptComponent* nsc) {delete nsc->instance; nsc->instance = nullptr; };
		}
	};


	struct cameraProjectionDirty{
		uint8_t dummy = 0;

	};
	

}