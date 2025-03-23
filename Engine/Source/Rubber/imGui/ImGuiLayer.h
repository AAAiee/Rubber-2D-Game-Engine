#pragma once

#include "Rubber/Layer.h"

namespace Rubber
{
	class RB_API ImGuiLayer :public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();


		void onAttach() override;
		void onDetach() override;
		void onUpdate() override;
		void onEvent(Event& event)  override;

	private:
		float m_Time;
	};
}

