#include <pch.h>
#include "GLFrameBuffer.h"
#include "glad/glad.h"


namespace Rubber{

	GLFrameBuffer::GLFrameBuffer(const FrameBufferSpecification& spec)
		:m_Spec(spec)
	{
		recreate();
	}

	GLFrameBuffer::~GLFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(1, &m_ColorAttachment);
		glDeleteTextures(1, &m_DepthAttachment);
	}

	void GLFrameBuffer::recreate()
	{
		if (m_RendererID  != 0 ){  // if there already exists a framebuffer, clean it up before recreating
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(1, &m_ColorAttachment);
			glDeleteTextures(1, &m_DepthAttachment);
		}

		glCreateFramebuffers(1, &m_RendererID);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
		glTextureStorage2D(m_ColorAttachment, 1, GL_RGBA8, m_Spec.m_Width, m_Spec.m_Height);

		glTextureParameteri(m_ColorAttachment, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_ColorAttachment, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glNamedFramebufferTexture(m_RendererID, GL_COLOR_ATTACHMENT0, m_ColorAttachment, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
		glTextureStorage2D(m_DepthAttachment, 1, GL_DEPTH24_STENCIL8, m_Spec.m_Width, m_Spec.m_Height);
		glNamedFramebufferTexture(m_RendererID, GL_DEPTH_STENCIL_ATTACHMENT, m_DepthAttachment, 0);

		glNamedFramebufferDrawBuffer(m_RendererID, GL_COLOR_ATTACHMENT0);
		RB_CORE_ASSERT(glCheckNamedFramebufferStatus(m_RendererID, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "incomplete frameBuffer!");
	}


	void GLFrameBuffer::resize(uint32_t width, uint32_t height)
	{
		m_Spec.m_Width = width;
		m_Spec.m_Height = height;
		recreate();
	}

	void GLFrameBuffer::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Spec.m_Width, m_Spec.m_Height);
	}

	void GLFrameBuffer::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}
