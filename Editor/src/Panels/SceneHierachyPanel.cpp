#include "Panels/SceneHierachyPanel.h"
#include <glm/gtc/type_ptr.hpp>
#include <entt.hpp>


namespace Rubber {

	SceneHierachyPanel::SceneHierachyPanel(const Ref<Scene>& scene)
	{
		setContext(scene);

	}

	void SceneHierachyPanel::setContext(const Ref<Scene>& scene)
	{
		m_Context = scene;
	}

	void SceneHierachyPanel::onImGuiRender()
	{
		{// Show all entities within a scene
			ImGui::Begin("Scene Hierarchy");
			auto view = m_Context->m_Registry.view<entt::entity>();
			for (auto entity : view) {

				drawEntityNode({ entity, m_Context });
			}

			// if left mouse button is pressed and not being consumed by other items, deselect
			if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(0)) {
				m_SelectionContext = {};
			}
			ImGui::End();

		}

		{// Show properties presented for a specific entity
			ImGui::Begin("Property Channel");
			if (m_SelectionContext) {
				drawComponentNode(m_SelectionContext);
			}
			ImGui::End();
		}

		//ImGui::ShowDemoWindow();
	} 

	void SceneHierachyPanel::drawEntityNode(Entity entity)
	{
		auto& tag = entity.getComponent<TagComponent>().tag; 
 
		// Only include the selected flag when the entity is selected
		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

		bool opened = ImGui::TreeNodeEx((void*)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked()) {
			m_SelectionContext = entity;
		}
		if (opened) {
			ImGui::TreePop();
		}
	}

	void SceneHierachyPanel::drawComponentNode(Entity entity) 
	{
		drawComponent<TagComponent>("Tag", entity, [](Entity entity)
			{ 
				std::string& tag = entity.getComponent<TagComponent>().tag;
				char buffer[256];
				memset(buffer, 0, sizeof(buffer));  //ensure clean buffer
				strcpy_s(buffer, sizeof(buffer), tag.c_str());
				if (ImGui::InputText("Tag", buffer, sizeof(buffer))) {
					tag = std::string(buffer);
				}
			});

		drawComponent<TransformComponent>("Transform", entity, [](Entity entity)
			{
				TransformComponent& tsC = entity.getComponent<TransformComponent>();
				ImGui::DragFloat3("Position", glm::value_ptr(tsC.position), 0.1f);
				ImGui::DragFloat("Rotation", &tsC.rotation);
				ImGui::DragFloat2("Scale", glm::value_ptr(tsC.scale), 0.1f);
			});


		drawComponent<CameraComponent>("Camera", entity, [](Entity entity)
			{
				
				CameraComponent& cc = entity.getComponent<CameraComponent>();
				SceneCamera& camera = cc.camera;

				ImGui::Checkbox("Primary", &cc.isPrimary);
				ImGui::Checkbox("IsFixedRatio", &cc.isFixedAspectRatio);

				const char* projectionTypeName[] = { "Orthographic", "Perspective" };
				const char* currentProjectionName = projectionTypeName[static_cast<uint8_t>(camera.getProjectionType())];

				if (ImGui::BeginCombo("Projection", currentProjectionName))
				{
					for (int i = 0; i < 2; i++) {

						bool isSelected = strcmp(currentProjectionName, projectionTypeName[i]);
						if (ImGui::Selectable(projectionTypeName[i], isSelected)) {
							currentProjectionName = projectionTypeName[i];
							camera.setProjectionType(static_cast<SceneCamera::ProjectionType>(i));
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();

					}
					ImGui::EndCombo();
				}

				switch (camera.getProjectionType()) {
					case SceneCamera::ProjectionType::Ortho:
					{
						float orthoSize = camera.getOrthoSize();
						if (ImGui::DragFloat("Size", &orthoSize)) {
							camera.setOrthoSize(orthoSize);
						}

						float orthoNearClip = camera.getOrthoNearClip();
						if (ImGui::DragFloat("NearClip", &orthoNearClip)) {
							camera.setOrthoNearClip(orthoNearClip);
						}

						float orthoFarClip = camera.getOrthoFarClip();
						if (ImGui::DragFloat("FarClip", &orthoFarClip)) {
							camera.setOrthoFarClip(orthoFarClip);
						}
						break;
					}

					case SceneCamera::ProjectionType::Persp:
					{
						float persFOVY = glm::degrees(camera.getPersFoVY());
						if (ImGui::DragFloat("FOVY", &persFOVY, 2.0f, 0.0f, 120.0f, "%.2f")) {
							camera.setPersFovY(glm::radians(persFOVY));
						}

						float persNearClip = camera.getPersNearClip();
						if (ImGui::DragFloat("NearClip", &persNearClip)) {
							camera.setPersNearClip(persNearClip);
						}

						float persFarClip = camera.getPersFarClip();
						if (ImGui::DragFloat("FarClip", &persFarClip)) {
							camera.setPersFarClip(persFarClip);
						}
						break;
					}
				}
			});


		drawComponent<SpriteComponent>("Sprite Renderable", entity, [](Entity entity) {
				auto& sprite = entity.getComponent<SpriteComponent>();

				ImGui::ColorEdit4("Color Picker", glm::value_ptr(sprite.color));
			});
	}
}
