#include "Rubber.h"
#include "Rubber/Core/EntryPoint.h"  
#include "exampleLayer.h"

class testEcs : public Rubber::Layer  
{  
public:  
    testEcs()
        :Layer("testECS")
    {
	

    }  

   void onUpdate() override  
   {  


   }  


   void onAttach(Ref<Rubber::EventManager> em){


   }


   void onDetach(Ref<Rubber::EventManager> em){


   }

};  



class test : public Rubber::Application {
public:
	test() {
		pushLayer(new testEcs());
		pushLayer(new ExampleLayer());

	}
	~test() {};

};


Rubber::Application* Rubber::CreateApp()  
{  
   return new test();  
}
