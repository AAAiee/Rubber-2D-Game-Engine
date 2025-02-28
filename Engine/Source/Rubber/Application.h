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
		// a pointer to the window object
		std::unique_ptr<Window> m_Window;
		// when the application is running
		bool m_Runing = true;
	};
	
}
