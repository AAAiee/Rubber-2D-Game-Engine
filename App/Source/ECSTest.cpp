//include the Entry Point  
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

   void onImGuiRender() override  
   {  

   }  

   void onEvent(Rubber::Event& event) override  
   {  

   }  
   

private:
    Rubber::EntityManager em;
	Rubber::testComponent comp;

  
};  

class test : public Rubber::Application{  
public:  
   test(){  
       pushLayer(new testEcs());
       pushLayer(new ExampleLayer());
   }  

   ~test() {};  

};  

Rubber::Application* Rubber::CreateApp()  
{  
   return new test();  
}
