#include "Rubber.h"
#include "Rubber/Core/EntryPoint.h"  
#include "GameLayer.h"



class Sanbox2D : public Rubber::Application {
public:
	Sanbox2D()
	   : Application("Demo")
	{
		pushLayer(new GameLayer());
	}
	~Sanbox2D() {};
};


Rubber::Application* Rubber::CreateApp()  
{  
   return new Sanbox2D();  
}
