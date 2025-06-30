#include "Rubber.h"
#include "Rubber/Core/EntryPoint.h"  
#include <glm/glm.hpp>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include "EditorLayer.h"



class Editor : public Rubber::Application {
public:
	Editor()
	  :Application("Editor")
	{
		pushLayer(new EditorLayer());

	}
	~Editor() {};
};


Rubber::Application* Rubber::CreateApp()  
{  
   return new Editor();  
}
