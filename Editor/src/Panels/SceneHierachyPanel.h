#pragma once

#include <Rubber.h>
#include <imgui.h>

namespace Rubber {

	class SceneHierachyPanel {
	public:
		SceneHierachyPanel() = default;

		SceneHierachyPanel(const Ref<Scene>& scene);

		void setContext(const Ref<Scene>& scene);

		void onImGuiRender();

	private:
		void drawEntityNode(Entity entity);
		void drawComponentNode(Entity entity);


		// template function used to draw different components

		//TODO: adding more parameter to control whether the component is removable or not
		template<typename T>
		void drawComponent(std::string_view name, Entity entity, void(*func)(Entity) , bool removable = true) {
			// allow overlap to make the button on the same line as the tree node
			ImGuiTreeNodeFlags flag = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

			if (entity.hasComponent<T>()) {
				bool isNodeOpen = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flag, name.data());
				ImGui::SameLine(ImGui::GetWindowWidth()  - 25.0f);

				// button 
				if (ImGui::Button("...", ImVec2{20, 20})) {
					ImGui::OpenPopup("ComponentSettings");
				}

				// should remove component
				bool shouldRemoveComponent = false;
				if (ImGui::BeginPopup("ComponentSettings")) {
					if (removable) {
						if (ImGui::MenuItem("Remove Component"))
							shouldRemoveComponent = true;
					}
					ImGui::EndPopup();
				}

				if (isNodeOpen) {
					func(entity);
					ImGui::TreePop();
				}

				if (shouldRemoveComponent)
					entity.removeComponent<T>();
			}
		}

	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};


}

