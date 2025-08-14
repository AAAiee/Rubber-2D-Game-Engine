#pragma once
#include <glm/glm.hpp>
#include <string>
#include <variant>
#include <Rubber/Renderer/Camera.h>
#include <Rubber/Scene/SceneCamera.h>
#include "Rubber/Animation/AnimationSpec.h"
#include <Rubber/Scene/Utili/ScriptableEntity.h>
#include "Rubber/Renderer/Texture.h"
#include "Rubber/Renderer/Subtexture2D.h"
#include "Rubber/Input/KeyCodes.h"


namespace Rubber{

	//*******************Transform**************************//
	struct TransformComponent{
		glm::vec3 position = { 0.0f, 0.0f, 0.0f };
		glm::vec2 scale{ 1.0f, 1.0f };
		float rotation{ 0.0f }; //radians

		TransformComponent(glm::vec3 position, glm::vec2 scale, float rotation)
			:position(position), scale(scale), rotation(rotation) { }

		TransformComponent() = default;

		glm::mat4 getTransformMatrix() const {
			glm::mat4 tsMatrix = glm::scale(
				glm::rotate( 
					glm::translate(glm::mat4(1.0f), position), rotation, glm::vec3(0, 0, 1)), glm::vec3(scale.x, scale.y, 1.0f));
			return tsMatrix;
		} ;

		~TransformComponent() = default;
		TransformComponent(const TransformComponent& other) = default;
	};


	//*******************Tag**************************//
	struct TagComponent{
		std::string tag;

		TagComponent(std::string_view tagName)
			:tag(tagName) {
		}
		~TagComponent() = default;
		TagComponent(const TagComponent& other) = default;
	}; 

	//*******************Sprite**************************//
	struct SpriteComponent{
		Ref<SubTexture2D>  texture = nullptr;
		glm::vec2  localOffset = { 0.0f, 0.0f };
		glm::vec4  color = { 1.0f, 1.0f,1.0f,1.0f };

		SpriteComponent() = default;
		SpriteComponent(const Ref<SubTexture2D>& tex) {
			texture = tex;
		}
		
		~SpriteComponent() = default;
		SpriteComponent(const SpriteComponent& other) = default;
	};

	//*******************Camera**************************//
	struct CameraComponent{
		SceneCamera camera;
		bool isFixedAspectRatio = false;
		bool isPrimary = false;
		
		CameraComponent(const SceneCamera& camera )
			:camera(camera) {
		}

		CameraComponent() = default;
		~CameraComponent() = default;
		CameraComponent(const CameraComponent& other) = default;
	};


	//*******************NativeScript**************************//

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

	//*******************Animation**************************//
	struct AnimationComponent {
		AnimationComponent(const AnimationClipConfig& config) 
			:specs(config)
		{

		}
		AnimationComponent() = default;

		AnimationComponent(const AnimationComponent& src) = default;
		AnimationSpec specs;
	};


	//*******************INPUT**************************//
	struct  KeyBinding {
		std::string name ;
		uint16_t keyCode = 0;
	};

	struct InputComponent {
		InputComponent(Vector<KeyBinding> keybindings) {
			bindings = std::move(keybindings);
		}

		InputComponent() = default;
		~InputComponent() = default;

		void setKeyBindings(Vector<KeyBinding> keybindings) {
			bindings = std::move(keybindings);
		}

		Vector<KeyBinding> bindings;
		std::unordered_map<std::string, bool> actionState;
		float mouseX = 0.0f;
		float mouseY = 0.0f;

		float mouseXWorld = 0.0f;
		float mouseYWorld = 0.0f;
	};


	//*******************STATE**************************//
	template<typename T>
	concept isStateEnum = std::is_enum_v<T>;

	template <isStateEnum T>
	struct Transition {
		T from;
		T to;
		std::function<bool(Entity)> condition;
	};

	template <isStateEnum T>
	struct StateMachineComponent {
		T currentState;
		T previousState;
		float elapsedTime = 0.0f;
		bool justEntered = true;

		Vector<Transition<T>> transitions;
		std::function<void(entt::entity, T)> onEnter;
		std::function<void(entt::entity, T)> onExit; 
		std::function<void(Entity, T, float)> onUpdate;
	};

	//*******************MoveComponent**************************//
	struct MoveComponent {
		glm::vec2 velocity{ 0.0f, 0.0f };

		float gravity = -9.8f;
		bool moveEnabled = true;
		bool enableGravity = true;
	};

	//*******************CollisionComponent**************************//

	enum class CollisionLayer: uint8_t {
		None,
		Enemy,
		Player
	};

	struct  CollisionComponent {
		glm::vec2 size     = { 1.0f, 1.0f };
		glm::vec2 position = { 0.0f, 0.0f };
		bool enabled = true;

		CollisionLayer srcLayer = CollisionLayer::None;
		CollisionLayer targetLayer = CollisionLayer::None;

		std::function<void(Entity)> onCollision;
	};
	
	struct CollisionBoxOnRenderFlag {
		bool onDebugRender = true;
		bool renderCollision = false;
	};


	struct VisibilityControlComponent {
		bool isVisible = true;
	};

}