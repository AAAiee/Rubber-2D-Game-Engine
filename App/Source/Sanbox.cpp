#include "Rubber.h"
#include <glm/glm.hpp>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include "Rubber/Core/EntryPoint.h"  
#include "EcsDevTest.h"
#include "exampleLayer.h"



class Sanbox2D : public Rubber::Application {
public:
	Sanbox2D()
	   : Application("Sandbox2D")
	{
		
		pushLayer(new ECSTestLayer());
		pushLayer(new ExampleLayer());

	}
	~Sanbox2D() {};
};


Rubber::Application* Rubber::CreateApp()  
{  
   return new Sanbox2D();  
}
