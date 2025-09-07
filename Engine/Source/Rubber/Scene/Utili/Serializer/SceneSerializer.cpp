#include <pch.h>

#include "Rubber/Scene/Scene.h"
#include "Rubber/Scene/Utili/Entity.h"
#include "Rubber/Scene/Utili/Component.h"

#define YAML_CPP_STATIC_DEFINE
#include "SceneSerializer.h"
#include <yaml-cpp/yaml.h>


namespace YAML { // overloads for glm types

	template<>
	struct convert<glm::vec3> {
		static Node encode(const glm::vec3& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs) {
			if (!node.IsSequence() || node.size() != 3) {
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};


	template<>
	struct convert<glm::vec4> {
		static Node encode(const glm::vec4& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs) {
			if (!node.IsSequence() || node.size() != 4) {
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

namespace Rubber {

	namespace { // serialization helpers
		// overload for custom types
		YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& vec) {
			out << YAML::Flow;
			out << YAML::BeginSeq << vec.x << vec.y << vec.z << YAML::EndSeq;
			return out;
		}

		YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& vec) {
			out << YAML::Flow;
			out << YAML::BeginSeq << vec.x << vec.y << vec.z << vec.w << YAML::EndSeq;
			return out;
		}


		template<typename ComponentType, typename ComponentFunc>
		void serializeComponent(YAML::Emitter& out, Entity entity,std::string_view componentName, ComponentFunc func) { 
			if (entity.hasComponent<ComponentType>()) {
				out << YAML::Key << componentName;
				out << YAML::BeginMap;
				const auto& component = entity.getComponent<ComponentType>();
				func(out, component);
				out << YAML::EndMap;
			}
		}

		void serializeEntity(YAML::Emitter& out, Entity entity) {
			out << YAML::BeginMap; // entity
			out << YAML::Key << "Entity" << YAML::Value << 123123141; // TODO:UUID

			//tag component
			serializeComponent<TagComponent>(out, entity, "TagComponent", [](YAML::Emitter& out, const TagComponent& component) {
				out << YAML::Key << "Tag" << YAML::Value << component.tag;
			});

			serializeComponent<TransformComponent>(out, entity, "TransformComponent", [](YAML::Emitter& out, const TransformComponent& component) {
				out << YAML::Key << "Position" << YAML::Value << component.position;
				out << YAML::Key << "Rotation" << YAML::Value << component.rotation;
				out << YAML::Key << "Scale" << YAML::Value << component.scale;
			});

			serializeComponent<CameraComponent>(out, entity, "CameraComponent", [](YAML::Emitter& out, const CameraComponent& component) {
				const SceneCamera& camera = component.camera;
     
				out << YAML::Key << "Camera"  << YAML::Value  << YAML::BeginMap;
				{
					out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.getProjectionType();
					out << YAML::Key << "PerspectiveFovY" << YAML::Value << camera.getPersFoVY();
					out << YAML::Key << "PerspectiveNearClip" << YAML::Value << camera.getPersNearClip();
					out << YAML::Key << "PerspectiveFarClip" << YAML::Value << camera.getPersFarClip();
					out << YAML::Key << "OrthographicSize" << YAML::Value << camera.getOrthoSize();
					out << YAML::Key << "OrthographicNearClip" << YAML::Value << camera.getOrthoNearClip();
					out << YAML::Key << "OrthographicFarClip" << YAML::Value << camera.getOrthoFarClip();
				}
				out << YAML::EndMap;

				out << YAML::Key << "Primary" << YAML::Value << component.isPrimary;
				out << YAML::Key << "FixedAspectRatio" << YAML::Value << component.isFixedAspectRatio;
			});

			serializeComponent<SpriteComponent>(out, entity, "SpriteComponent", [](YAML::Emitter& out, const SpriteComponent& component) {
				out << YAML::Key << "Color" << YAML::Value << component.color;
			});

			serializeComponent<VisibilityControlComponent>(out, entity, "VisibilityControl", [](YAML::Emitter& out, const VisibilityControlComponent& component) {
				out << YAML::Key << "IsVisible" << YAML::Value << component.isVisible;
				});

			out << YAML::EndMap;
		}

		void saveToFile(const YAML::Emitter& out, const std::filesystem::path& filepath) {

			// if has the parent path, create directories
			if (!filepath.parent_path().empty())
				std::filesystem::create_directories(filepath.parent_path());

			std::ofstream fout(filepath);

			if (!fout) {
				throw std::runtime_error("Could not open file for writing: " + filepath.string());
			}

			fout << out.c_str();
		}
	} // namespace serialization helpers


	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		:m_Context(scene) { }

	void SceneSerializer::serialize(const std::filesystem::path& filepath)
	{
		YAML::Emitter out;

		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";

		// serialize all entities
		out << YAML::Key << "Entities" << YAML::BeginSeq;
		for (auto entityID: m_Context->m_Registry.storage<entt::entity>()) {

			Entity entity = { entityID, m_Context};
			
			if (!entity)
				continue;

			serializeEntity(out, entity);
		}

		out << YAML::EndSeq;
		out << YAML::EndMap;

		saveToFile(out, filepath);
	}

	void SceneSerializer::serializeRunTime(const std::filesystem::path& filepath)
	{
		RB_CORE_ASSERT(false);
	}

	bool SceneSerializer::deserialize(const std::filesystem::path& filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		RB_INFO("Deserializing scene: {0}", sceneName);

		auto entities = data["Entities"];
		if (entities) {
			for (const auto& entity : entities) {
				
				uint64_t uuid = entity["Entity"].as<uint64_t>(); //TODO:: ADD UUID
				std::string name;

				auto tagComponent = entity["TagComponent"];
				if (tagComponent) {
					name = tagComponent["Tag"].as<std::string>();
				}
				RB_INFO("Deserialized entity with ID: {0}, name: {1}", uuid, name);
				Entity deserializedEntity = m_Context->createEntity(name);

				{//deserialize transform Component
					auto transformComponent = entity["TransformComponent"];
					if (transformComponent) {
						auto& tc = deserializedEntity.addComponent<TransformComponent>();
						tc.position = transformComponent["Position"].as<glm::vec3>();
						tc.rotation = transformComponent["Rotation"].as<glm::vec3>();
						tc.scale = transformComponent["Scale"].as<glm::vec3>();
					}
					RB_INFO("Deserialized Transform Component");
				}// deserialize transform Component


				{ // deserialize Camera Component
					auto cameraComponent = entity["CameraComponent"];
					if (cameraComponent) {
						auto& cc = deserializedEntity.addComponent<CameraComponent>();
						auto cameraProps = cameraComponent["Camera"];
						cc.camera.setProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());
						cc.camera.setPersFovY(cameraProps["PerspectiveFovY"].as<float>());
						cc.camera.setPersNearClip(cameraProps["PerspectiveNearClip"].as<float>());
						cc.camera.setPersFarClip(cameraProps["PerspectiveFarClip"].as<float>());
						cc.camera.setOrthoSize(cameraProps["OrthographicSize"].as<float>());
						cc.camera.setOrthoNearClip(cameraProps["OrthographicNearClip"].as<float>());
						cc.camera.setOrthoFarClip(cameraProps["OrthographicFarClip"].as<float>());
						cc.isPrimary = cameraComponent["Primary"].as<bool>();
						cc.isFixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
					}
					RB_INFO("Deserialized Camera Component");
				} // deserialize Camera Component


				{//deserialize Sprite Component
					auto spriteComponent = entity["SpriteComponent"];
					if (spriteComponent) {
						auto& sc = deserializedEntity.addComponent<SpriteComponent>();
						sc.color = spriteComponent["Color"].as<glm::vec4>();
					}

					RB_INFO("Deserialized Sprite Component");
				} // deserialize Sprite Component

				{// deserialize Visibility Control Component
					auto visibilityControlComponent = entity["VisibilityControl"];
					if (visibilityControlComponent) {
						auto& vcc = deserializedEntity.addComponent<VisibilityControlComponent>();
						vcc.isVisible = visibilityControlComponent["IsVisible"].as<bool>();
					}

					RB_INFO("Deserialized Visibility Control Component");
				} //deserialize Visibility Control Component
			}
		}
		return true;
	}

	bool SceneSerializer::deserializeRunTime(const std::filesystem::path& filepath)
	{

		RB_CORE_ASSERT(false);
		return false;
	}

	
}


