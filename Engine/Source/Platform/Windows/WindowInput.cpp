#include "pch.h"  
#include "WindowInput.h"  
#include <GLFW/glfw3.h>
#include "Rubber/Application.h"
#include "Rubber/Window.h"

namespace Rubber {  
   Input* Input::s_Instance = new WindowInput();
   
   bool WindowInput::isKeyPressedImpl(int keyCode){
	   auto win =static_cast<GLFWwindow*>(Application::getWindow().getNativeWindow());
	   bool state = glfwGetKey(win, keyCode);
	   return state;
   }  

   bool WindowInput::isMouseButtonPressedImpl(int button) {
	   auto win = static_cast<GLFWwindow*>(Application::getWindow().getNativeWindow());
	   bool state = glfwGetKey(win, button);
	   return state;
   }  

   std::pair<float, float> WindowInput::getMousePositionImpl(){  
	   auto win = static_cast<GLFWwindow*>(Application::getWindow().getNativeWindow());
	   double xPos, yPos;
	   glfwGetCursorPos(win, &xPos, &yPos);
	   return { (float)xPos, (float)yPos };
   }
}
