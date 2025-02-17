#pragma once
#include <Rubber/Core.h>
#include <Rubber/Window.h>

namespace Rubber
{
	/* Application
	*  this class controls the application 
	*/
	class RB_API Application
	{
	public:
		Application();
		virtual ~Application();
		void run();
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Runing = true;
	};
	
}
