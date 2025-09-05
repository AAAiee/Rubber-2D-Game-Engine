#pragma once
#include "Rubber/Core/Core.h"
#include "Rubber/Event/EventManager.h"

namespace Rubber
{
	class Event;

	struct WindowProps
	// a sturct to organize window properties, with default value being set
	{
		std::string title;
		unsigned int width;
		unsigned int height;

		WindowProps(std::string_view title = "Rubber Engine", unsigned int width = 1600, unsigned int height = 900)
			:title(title), width(width), height(height)
		{

		}

	};
	class  Window
	//base class for all windows on different platforms
	{
	public:
		//destructor
		virtual ~Window() {};

		virtual void onUpdate() = 0;

		//Window properties
		virtual unsigned int getWidth() const = 0;
		virtual unsigned int getHeight() const = 0;
		virtual void* getNativeWindow() const = 0;

		//Window Attributes 
		virtual void setEventManager(Ref<EventManager>& em) = 0;
		virtual void setVsync(bool enabled) = 0;
		virtual bool isVsync() const = 0;

		//create a window depends on the different platform
		static Scope<Window> create(const WindowProps & = WindowProps());
	};
}