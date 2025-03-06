#pragma once
#include "Rubber/Core.h"
#include "Rubber/Layer.h"

namespace Rubber {
	class RB_API LayerStack
		// a wrapper class for vector, which is the
		// container for our Layers 
	{
	public:
		LayerStack();
		~LayerStack();

		void pushLayer(Layer* layer);
		void popLayer(Layer* layer);
		void pushOverlay(Layer* layer);
		void popOverlay(Layer* layer);


		inline std::vector<Layer*>::iterator begin() {
			return m_Layers.begin();
		}

		inline std::vector<Layer*>::iterator end() {
			return m_Layers.end();
		}

	private:

		// a contiguous container to store our layers
		std::vector<Layer*> m_Layers;

		// using vector because we want to
		// insert layer in the middle, 
		// Overlay at the last,
		// also need reverse iterator
		std::size_t m_LayerInsertIndex;
	};
}

