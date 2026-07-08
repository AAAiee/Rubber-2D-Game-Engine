#pragma once
#include "RB/Core/Core.h"
#include "RB/input/Input.h"

struct GLFWwindow;
namespace RB {
	/**
	* @brief WindowInput class is a subclass of Input class.
	* It provides specific implementation for the input system.
	* Windows window implementation now dependent on GLFW.
	*/
	class  WindowInput :public Input
	{
	protected:
		virtual bool isKeyPressedImpl(int keyCode)  final override;
		virtual bool isMouseButtonPressedImpl(int button) final override;
		virtual std::pair<float, float> getMousePositionImpl() final override;
		virtual float getMouseXImpl()  final override { return getMousePositionImpl().first; }
		virtual float getMouseYImpl()  final override { return getMousePositionImpl().second; }
		virtual void disableCursorImpl() final override;
		virtual void enableCursorImpl() final override;
	};
}

