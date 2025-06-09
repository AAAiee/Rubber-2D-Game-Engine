#include "pch.h"
#include "GLBuffers.h"
#include <glad/glad.h>
#include <Rubber/Core/Assert.h>

namespace Rubber {

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------- Vertex Buffer ---------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
	GLVertexBuffer::GLVertexBuffer(float* vertices, size_t size){
		glCreateBuffers(1, &this->m_RendererID);

// 3.3		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

		glNamedBufferData(m_RendererID, size, vertices, GL_STATIC_DRAW);
	}

	void GLVertexBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, this->m_RendererID);
	}

	void GLVertexBuffer::unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void GLVertexBuffer::setLayout(const BufferLayout& layout)
	{
		this->m_layout = layout;
	}

	inline const BufferLayout& GLVertexBuffer::getLayout() const
	{
		RB_CORE_ASSERT(m_layout.getElementBuffer().size(), "The layout is not set yet, make sure you set before get");
		return this->m_layout;
	}

	GLVertexBuffer::~GLVertexBuffer()
	{
		glDeleteBuffers(1, &this->m_RendererID);
	}

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------- Index Buffer ---------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
	GLIndexBuffer::GLIndexBuffer(uint32_t*indices, size_t size)
	{
		glCreateBuffers(1, &this->m_RendererID);
		// 3.3 glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
		glNamedBufferData(m_RendererID, size, indices, GL_STATIC_DRAW);
		this->m_Count = static_cast<uint32_t> (size / sizeof(uint32_t));
	}

	void GLIndexBuffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_RendererID);
	}

	void GLIndexBuffer::unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

    GLIndexBuffer::~GLIndexBuffer()
	{
		glDeleteBuffers(1, &this->m_RendererID);

	}
}
