#include "Rubber.h"
#include "Rubber/Core/EntryPoint.h"  
#include "exampleLayer.h"

class testEcs : public Rubber::Layer  
{  
public:  
    testEcs()
        :Layer("testECS")
    {
		Rubber::Registry world; 
		Rubber::Entity testEnt = world.CreateEntity(); 
		glm::vec3 position = { 0.2f, 0.3f, -0.1f };
		glm::quat rotation = { 1.0f, 0.0f, 0.0f, 0.0f };
		glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

		world.emplace<Rubber::TransformStorage>(testEnt, position, rotation, scale);
		
		//
		Rubber::TransformStorage& ts = world.getStorage<Rubber::TransformStorage>();

		//Rubber::Group<TransformStorage, SpriteStorage>& transformSprite = world.getGroup< TransformStorage, SpriteStorage>;

		RB_CORE_ASSERT(ts.position(testEnt) == position, "fail");
		RB_CORE_ASSERT(ts.rotation(testEnt) == rotation, "fail");
		RB_CORE_ASSERT(ts.scale(testEnt) == scale, "fail");
		
		world.alive(testEnt);
		
		world.destroy(testEnt);

		world.flush();

		RB_CORE_ASSERT(world.alive(testEnt) == false, "fail");
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
