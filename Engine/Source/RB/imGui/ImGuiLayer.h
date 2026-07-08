#pragma once

#include "RB/Layer/Layer.h"

#include "RB/Event/Event.h"
#include "RB/Event/KeyEvent.h"
#include "RB/Event/MouseEvent.h"
struct GLFWwindow;

namespace RB
{
	/**
	* Dear ImGui Layer, currently implementation 
	* depends on GLfW and OpenGL.
	*/
	class EventManager;
	class  ImGuiLayer :public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		/**
		* @breif attach the layer to the application.
		* initialize the ImGui context and set up the style,
		* also enable mouse and keyboard navigation
		*/
		virtual void onAttach()  final override;

		/**
		* @brief detach the layer from the application.
		* 
		*/ 
		virtual void onDetach() final override;

		virtual void  onImGuiRender() final override;
		void begin();
		void end();


	private:
		float m_Time = 0.0f;

	};
}

