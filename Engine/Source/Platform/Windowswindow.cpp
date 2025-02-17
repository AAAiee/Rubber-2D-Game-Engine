#include "pch.h"
#include "WindowsWindow.h"
namespace Rubber {
	// false by default, window not initialized yet
	bool WindowsWindow::s_GLFWInitialized = false;

	//constructor delegate initialization to init
	Rubber::WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		init(props);
	}

	Rubber::WindowsWindow::~WindowsWindow()
	{
	}

	void Rubber::WindowsWindow::onUpdate()
	{
	}

	void Rubber::WindowsWindow::setVsync(bool enabled)
	{
	}

	bool Rubber::WindowsWindow::isVsync() const
	{
		return false;
	}

	void Rubber::WindowsWindow::init(const WindowProps& props)
	{
	}

	void Rubber::WindowsWindow::shutDown()
	{
	}
}
