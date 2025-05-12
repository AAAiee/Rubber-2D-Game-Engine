#include<Rubber.h>
#include <glm/glm.hpp>
#include <imgui.h>

class ExampleLayer : public Rubber::Layer
{
public:
	ExampleLayer()
		:Layer("example")
	{
	}

	void onUpdate() override
	{

	}

	void onImGuiRender() override
	{
		ImGui::Begin("Example Layer");
		ImGui::Text("Hello from ExampleLayer");
		ImGui::End();
	}

	void onEvent(Rubber::Event& event) override
	{
		if (event.getEventType() == Rubber::EventType::KeyboardPressed)
		{
			Rubber::KeyPressedEvent& e = (Rubber::KeyPressedEvent&)event;
			RB_INFO("pressing tab key? :{0} ", e.getKeyCode() == RB_KEY_TAB);
		}
	}

};
		
	

class Sandbox :public Rubber::Application
{
public:

	Sandbox()
	{
		pushLayer(new ExampleLayer());
	}
	~Sandbox()
	{
			
	}
	
};


Rubber::Application* Rubber::CreateApp()
{
	return new Sandbox();
}

