#pragma once

#include "Rubber/Layer.h"

#include "Rubber/Event/Event.h"
#include "Rubber/Event/KeyEvent.h"
#include "Rubber/Event/MouseEvent.h"
struct GLFWwindow;

namespace Rubber
{
	/**
	* Dear ImGui Layer, currently implementation 
	* depends on GLfW and OpenGL.
	*/
	class RB_API ImGuiLayer :public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		/**
		* @breif attach the layer to the application.
		* initialize the ImGui context and set up the style,
		* also enable mouse and keyboard navigation
		*/
		void onAttach() override;

		/**
		* @brief detach the layer from the application.
		* 
		*/ 
		void onDetach() override;

		/**
		* @brief update the layer,rendering ImGui on screen.
		*/
		void onUpdate() override;


	private:
		float m_Time = 0;

		// glfw window where imgui will be rendered on. 
		GLFWwindow* m_glfwWindow = nullptr;
	};
}

