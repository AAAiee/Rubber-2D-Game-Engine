#pragma once
#include "Rubber/Utility/Utility.h"

namespace Rubber {
	class Layer;
	class  LayerStack
		// a wrapper class for vector, which is the
		// container for our Layers 
		// all layers will be destroyed when program ends
		// pop layers does not free the layer immediately
		// because layers are supposed to live til program ends
	{
	public:
		LayerStack();
		~LayerStack();

		// push the layers in, but always before overlay
		void pushLayer(Layer* layer);
		
		// pop the specific layer if it is in there
		// the layer poped is not freed
		void popLayer(Layer* layer);

		// push overlay to the end;
		void pushOverlay(Layer* layer);

		// pop overlay if exists
		// notice that the layer is not freed after pop
		void popOverlay(Layer* layer);

		// implement begin and end iterator to support traversing the layers
		Vector<Layer*>::iterator begin();

		Vector<Layer*>::iterator end();

	private:

		// a contiguous container to store our layers
		Vector<Layer*> m_Layers;

		// using vector because we want to
		// insert layer in the middle, 
		// Overlay at the last,
		// also need reverse iterator for handling events
		unsigned int m_LayerInsertIndex;
	};
}

