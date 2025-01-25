#pragma once
#include <Rubber/Core.h>

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
	};
	
}
