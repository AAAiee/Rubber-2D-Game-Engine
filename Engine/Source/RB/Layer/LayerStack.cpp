#include <pch.h>
#include "LayerStack.h"
#include "RB/Layer/Layer.h"

RB::LayerStack::LayerStack()
	:m_LayerInsertIndex(0)
{
}

RB::LayerStack::~LayerStack()
{
	for (Layer* layer : m_Layers)
	{
		delete layer;
	}
}

void RB::LayerStack::pushLayer(Layer* layer)
{
  m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
  m_LayerInsertIndex++;
}

void RB::LayerStack::popLayer(Layer* layer)
{
	auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);
	if (it != m_Layers.end())
	{
       m_Layers.erase(it);
       m_LayerInsertIndex--;
	}
}

void RB::LayerStack::pushOverlay(Layer* overlay)
{
	m_Layers.emplace_back(overlay);
}

void RB::LayerStack::popOverlay(Layer* overlay)
{
	auto it = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), overlay);
	if (it != m_Layers.end())
	{
		m_Layers.erase(it);
	}
}

// implement begin and end iterator to support enforced loop reference 
RB::Vector<RB::Layer*>::iterator RB::LayerStack::begin() {
	return m_Layers.begin();
}

RB::Vector<RB::Layer*>::iterator RB::LayerStack::end() {
	return m_Layers.end();
}
