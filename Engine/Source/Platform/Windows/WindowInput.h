#pragma once
#include "Rubber/Core.h"
#include "Rubber/input/Input.h"

struct GLFWwindow;
namespace Rubber {
	/**
	* @brief WindowInput class is a subclass of Input class.
	* It provides specific implmentation for the input system.
	* Windows window implmentation now dependent on GLFW.
	*/
	class RB_API WindowInput :public Input
	{
	protected:
		virtual bool isKeyPressedImpl(int keyCode)  final override;
		virtual bool isMouseButtonPressedImpl(int button) final override;
		virtual std::pair<float, float> getMousePositionImpl() final override;
		virtual float getMouseXImpl()  final override { return getMousePositionImpl().first; }
		virtual float getMouseYImpl()  final override { return getMousePositionImpl().second; }
	};
}

