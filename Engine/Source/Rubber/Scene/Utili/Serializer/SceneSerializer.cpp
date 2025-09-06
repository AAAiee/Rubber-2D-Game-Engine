#include <pch.h>

#include "Rubber/Scene/Scene.h"
#include "Rubber/Scene/Utili/Entity.h"
#include "Rubber/Scene/Utili/Component.h"

#define YAML_CPP_STATIC_DEFINE
#include "SceneSerializer.h"
#include <yaml-cpp/yaml.h>


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
	}

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
		return false;
	}

	bool SceneSerializer::deserializeRunTime(const std::filesystem::path& filepath)
	{

		RB_CORE_ASSERT(false);
		return false;
	}

	
}


