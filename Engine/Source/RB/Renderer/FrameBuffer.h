#pragma once
#include "RB/Renderer/Texture.h"
#include <RB/Utility/Utility.h>

namespace RB{
	
	struct FrameBufferSpecification{
		uint32_t m_Width=1280, m_Height=720; 
		uint32_t m_Samples = 1; 

		bool m_SwapChainTarget = false;
	}; 

	class FrameBuffer{
	public:
		virtual ~FrameBuffer() {};
		virtual const FrameBufferSpecification& getSpecification() const = 0;
		virtual void bind() = 0;
		virtual void unbind() = 0;
		virtual const uint32_t getRendererId() const = 0;
		virtual const uint32_t getColorAttachmentID() const = 0;
		virtual const uint32_t getDepthAttachmentID() const = 0;
		virtual void resize(uint32_t width, uint32_t height) = 0;

	public:
		static Ref<FrameBuffer> create(const FrameBufferSpecification& spec); 
	};


}