#pragma once
#include "Rubber/Core/Core.h"
#include <utility>

namespace Rubber {
	/**
	* @breif Input class can be used to track the status 
	* of key or mouse button. it is meant to be a singleton
	* its s_Instance is created in the WindowInput class for Windows
	* implmentation. The specific implementation is platform dependent,
	* which is why it has protected virtual functions that can adopt
	* different implmentation.
	*/ 
	class RB_API Input
	{
	public:
		static inline bool isKeyPressed(int keyCode) {
			return s_Instance->isKeyPressedImpl(keyCode);
		}
		static inline  bool isMouseButtonPressed(int button) {
			return s_Instance->isMouseButtonPressedImpl(button);
		}
		static inline std::pair<float, float> getMousePosition() {
			return s_Instance->getMousePositionImpl();
		}
		static inline float getMouseX(int button) {
			return s_Instance->getMouseXImpl();
		}

		static inline float getMouseY(int button) {
			return s_Instance->getMouseYImpl();
		}

	protected:
		virtual bool isKeyPressedImpl(int keyCode) = 0;
		virtual bool isMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> getMousePositionImpl() = 0;
		virtual float getMouseXImpl() = 0;
		virtual float getMouseYImpl() = 0;

	private:
		static  Input* s_Instance ;
	};
}
