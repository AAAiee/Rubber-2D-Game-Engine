#pragma once	
namespace RB {
	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;
		virtual void init() = 0;
		virtual void swapBuffer() = 0;
	};
}

