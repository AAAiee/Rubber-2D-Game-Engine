#include "Rubber.h"
#include "Rubber/Core/EntryPoint.h"  
#include <glm/glm.hpp>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include "EditorLayer.h"



namespace Rubber {
	class Editor : public Application {
	public:
		Editor()
			:Application("Editor")
		{
			pushLayer(new EditorLayer());

		}
		~Editor() {};
	};


	Application* CreateApp()
	{
		return new Editor();
	}
}
