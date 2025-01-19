#include<Client.h>
class Sandbox :public Rubber::Application
{
public:

	Sandbox()
	{
		
	}
	~Sandbox()
	{
			
	}
	
};


Rubber::Application* Rubber::CreateApp()
{
	return new Sandbox();
}

