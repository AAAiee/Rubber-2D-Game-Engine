#include<Client.h>

class ExampleLayer : public Rubber::Layer
{
public:
	ExampleLayer()
		:Layer("example")
	{
	}

	void onUpdate() override
	{
		RB_INFO("ExampleLayer::Update");
	}

	void onEvent(Rubber::Event& event) override
	{
		RB_TRACE("{0}", event.toString());
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

