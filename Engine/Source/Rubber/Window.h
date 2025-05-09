#pragma once
#include "Rubber/Core.h"
namespace Rubber
{
	class Event;

	struct WindowProps
	// a sturct to organize window properties, with default value being set
	{
		std::string title;
		unsigned int width;
		unsigned int height;

		WindowProps(const std::string& title = "Rubber Engine", unsigned int width = 1280, unsigned int height = 720)
			:title(title), width(width), height(height)
		{

		}

	};
	class RB_API Window
	//base class for all windows on different platforms
	{
	public:
		// define function pointer type for EventCallBack function
		using EventCallBackFn = std::function<void(Event&)>;

		//destructor
		virtual ~Window() {};

		virtual void onUpdate() = 0;

		//Window propeties
		virtual unsigned int getWidth() const = 0;
		virtual unsigned int getHeight() const = 0;
		virtual void* getNativeWindow() const = 0;

		//Window Attributes 
		virtual void setEventCallBack(const EventCallBackFn& callback) = 0;
		virtual void setVsync(bool enabled) = 0;
		virtual bool isVsync() const = 0;

		//create a window depends on the different platform
		static std::unique_ptr<Window> create(const WindowProps & = WindowProps());
	};
}