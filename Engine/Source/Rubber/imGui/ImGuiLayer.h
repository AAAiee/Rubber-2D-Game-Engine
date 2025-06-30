#pragma once

#include "Rubber/Layer/Layer.h"

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
	class EventManager;
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
		virtual void onAttach(const Ref<EventManager>& em) final override;

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

