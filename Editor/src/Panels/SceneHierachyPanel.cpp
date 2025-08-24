#include "Panels/SceneHierachyPanel.h"
#include <glm/gtc/type_ptr.hpp>
#include <entt.hpp>



 namespace {

	 void drawVec3Control(std::string_view label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 70.0f) {
		 ImGui::PushID(label.data()); // Avoid ID collisions
		 if (ImGui::BeginTable("vec3Control", 2, ImGuiTableFlags_SizingStretchProp)) {
			 ImGui::TableSetupColumn("LabelCol", ImGuiTableColumnFlags_WidthFixed, columnWidth);
			 ImGui::TableSetupColumn("ControlCol", ImGuiTableColumnFlags_WidthStretch);

			 ImGui::TableNextRow();

			 // Label
			 ImGui::TableSetColumnIndex(0);
			 ImGui::Text("%s", label.data());

			 // Controls
			 ImGui::TableSetColumnIndex(1);
			 float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
			 ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

			 ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0)); // No gaps between button & drag
			 ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x / 3.0f - buttonSize.x);

			 // --- X ---
			 ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
			 ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
			 ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
			 if (ImGui::Button("X", buttonSize)) values.x = resetValue;
			 ImGui::SameLine();
			 ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
			 ImGui::PopStyleColor(3);

			 // --- Y ---
			 ImGui::SameLine();
			 ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
			 ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
			 ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
			 if (ImGui::Button("Y", buttonSize)) values.y = resetValue;
			 ImGui::SameLine();
			 ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
			 ImGui::PopStyleColor(3);

			 // --- Z ---
			 ImGui::SameLine();
			 ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
			 ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.35f, 0.9f, 1.0f));
			 ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
			 if (ImGui::Button("Z", buttonSize)) values.z = resetValue;
			 ImGui::SameLine();
			 ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
			 ImGui::PopStyleColor(3);

			 ImGui::PopItemWidth();
			 ImGui::PopStyleVar();

			 ImGui::EndTable();
		 }
		 ImGui::PopID();
	 }
}

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

			// If left mouse button is pressed and not being consumed by other items, de-select
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
	} 

	void SceneHierachyPanel::drawEntityNode(Entity entity)
	{
		auto& tag = entity.getComponent<TagComponent>().tag; 
 
		// Only include the selected flag when the entity is selected
		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

		// Draw Entity Node
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
				// Use a char buffer to store the tag string temporarily
				std::string& tag = entity.getComponent<TagComponent>().tag;
				char buffer[256];
				memset(buffer, 0, sizeof(buffer));  //ensure clean buffer
				strcpy_s(buffer, sizeof(buffer), tag.c_str());

				// output the buffer's content into the input text field
				if (ImGui::InputText("Tag", buffer, sizeof(buffer))) {
					tag = std::string(buffer);
				}
			});

		drawComponent<TransformComponent>("Transform", entity, [](Entity entity)
			{
				// Position, rotation, scale drag bar
				TransformComponent& tsC = entity.getComponent<TransformComponent>();

				drawVec3Control("Position", tsC.position);
				glm::vec3 rotation = glm::degrees(tsC.rotation);
				drawVec3Control("Rotation", rotation);
				tsC.rotation = glm::radians(rotation);
				drawVec3Control("Scale", tsC.scale);
			});


		drawComponent<CameraComponent>("Camera", entity, [](Entity entity)
			{
				auto& cc = entity.getComponent<CameraComponent>();
				SceneCamera& camera = cc.camera;

				// Two check boxes for  primary and fixed aspect ratio properties for each camera 
				ImGui::Checkbox("Primary", &cc.isPrimary);
				ImGui::Checkbox("IsFixedRatio", &cc.isFixedAspectRatio);

				const char* projectionTypeName[] = { "Orthographic", "Perspective" };
				const char* currentProjectionName = projectionTypeName[static_cast<uint8_t>(camera.getProjectionType())];

				if (ImGui::BeginCombo("Projection", currentProjectionName))
				{
					for (int i = 0; i < 2; i++) {

						bool isSelected = strcmp(currentProjectionName, projectionTypeName[i]) == 0;
						if (ImGui::Selectable(projectionTypeName[i], isSelected)) {
							currentProjectionName = projectionTypeName[i];
							camera.setProjectionType(static_cast<SceneCamera::ProjectionType>(i));
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				// Show specifications of each camera 
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


		// Only supports a color picker for a color renderable sprite component.
		drawComponent<SpriteComponent>("Sprite Renderable", entity, [](Entity entity) {
				auto& sprite = entity.getComponent<SpriteComponent>();
				ImGui::ColorEdit4("Color Picker", glm::value_ptr(sprite.color));
			});
	}
}
