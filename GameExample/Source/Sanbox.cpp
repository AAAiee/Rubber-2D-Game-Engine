#include "RB.h"
#include "RB/Core/EntryPoint.h"  
#include "GameLayer.h"



class Sanbox2D : public RB::Application {
public:
	Sanbox2D()
	   : Application("Demo")
	{
		pushLayer(new GameLayer());
	}
	~Sanbox2D() {};
};


RB::Application* RB::CreateApp()  
{  
   return new Sanbox2D();  
}
