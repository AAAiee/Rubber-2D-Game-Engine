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


	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};


}

