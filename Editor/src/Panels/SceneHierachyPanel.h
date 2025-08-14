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

		template<typename T>
		void drawComponent(std::string_view name, Entity entity, void(*func)(Entity)) {
			if (entity.hasComponent<T>()) {
				bool isNodeOpen = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, name.data());
				if (isNodeOpen) {
					func(entity);
					ImGui::TreePop();
				}
			}
		}


	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};


}

