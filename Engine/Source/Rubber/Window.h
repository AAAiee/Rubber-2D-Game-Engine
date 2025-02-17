#pragma once
#include <pch.h>
#include "Rubber/Core.h"
#include "Rubber/Event/Event.h"
namespace Rubber
{

	struct WindowProps
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
	{
	public:
		using EventCallBackFn = std::function<void(Event&)>;

		virtual ~Window() {};


		virtual void onUpdate() = 0;

		virtual unsigned int getWidth() const = 0;
		virtual unsigned int getHeight() const = 0;

		//Window Attributes 
		virtual void setEventCallBack(const EventCallBackFn* callback) = 0;
		virtual void setVsync(bool enabled) = 0;
		virtual bool isVsync() const = 0;


		//create a window depends on the different platform
		static Window* create(const WindowProps & = WindowProps());
	};
}