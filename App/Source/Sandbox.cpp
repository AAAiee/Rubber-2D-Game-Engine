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
		APP_INFO("ExampleLayer::Update");
	}

	void onEvent(Rubber::Event& event) override
	{
	    APP_TRACE("layer: {0}", event.toString());
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

