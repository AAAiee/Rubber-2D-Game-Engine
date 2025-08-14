#pragma once
#include "Rubber/Renderer/FrameBuffer.h"


namespace Rubber{
	class GLFrameBuffer: public FrameBuffer{
	public:
		GLFrameBuffer(const FrameBufferSpecification& spec);
		virtual ~GLFrameBuffer();
		virtual const FrameBufferSpecification& getSpecification() const final override {
			return m_Spec;
		};
		virtual const uint32_t getRendererId() const final override{
			return m_RendererID;
		}
		virtual const uint32_t getColorAttachmentID() const final override{
			return m_ColorAttachment;
		}
		virtual const uint32_t getDepthAttachmentID() const final override{
			return m_DepthAttachment;
		}


		virtual void recreate();
		virtual void resize(uint32_t width, uint32_t height) final override;
	public:
		virtual void bind() final override ;
		virtual void unbind() final override;

	private:
		uint32_t m_RendererID = 0;
		uint32_t m_ColorAttachment = 0;
		uint32_t m_DepthAttachment = 0;
		FrameBufferSpecification m_Spec;
	};

}