#include "pch.h"
#include "GLBuffers.h"
#include <glad/glad.h>
#include <Rubber/Core/Assert.h>

namespace Rubber {

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------- Vertex Buffer ---------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
	GLVertexBuffer::GLVertexBuffer(float* vertices, uint32_t size){
		RB_PROFILE_FUNC();
		glCreateBuffers(1, &m_RendererID);

// 3.3		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

		// for static data only
		glNamedBufferData(m_RendererID, size, vertices, GL_STATIC_DRAW);
	}

	GLVertexBuffer::GLVertexBuffer(uint32_t size)
	{
		RB_PROFILE_FUNC();

		// for batch rendering purpose, dynamically upload data to the buffer
		glCreateBuffers(1, &m_RendererID);
		glNamedBufferData(m_RendererID, size, nullptr, GL_DYNAMIC_DRAW);
	}

	void GLVertexBuffer::bind() const
	{
		RB_PROFILE_FUNC();

		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void GLVertexBuffer::unbind() const
	{
		RB_PROFILE_FUNC();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void GLVertexBuffer::setLayout(const BufferLayout& layout)
	{
		RB_PROFILE_FUNC();

		m_layout = layout;
	}

	void GLVertexBuffer::uploadVertexData(const void* const data, uint32_t size)
	{
		RB_PROFILE_FUNC();
		glNamedBufferData(m_RendererID, size, data, GL_DYNAMIC_DRAW);
	} 

	inline const BufferLayout& GLVertexBuffer::getLayout() const
	{
		RB_PROFILE_FUNC();

		RB_CORE_ASSERT(m_layout.getElementBuffer().size(), "The layout is not set yet, make sure you set before get");
		return m_layout;
	}

	GLVertexBuffer::~GLVertexBuffer()
	{
		RB_PROFILE_FUNC();

		glDeleteBuffers(1, &m_RendererID);
	}

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------- Index Buffer ---------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
	GLIndexBuffer::GLIndexBuffer(uint32_t*indices, uint32_t size)
	{
		RB_PROFILE_FUNC();

		glCreateBuffers(1, &m_RendererID);
		// 3.3 glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
		glNamedBufferData(m_RendererID, size, indices, GL_STATIC_DRAW);
		m_Count = static_cast<uint32_t> (size / sizeof(uint32_t));
	}

	GLIndexBuffer::GLIndexBuffer(uint32_t size)
	{
		RB_PROFILE_FUNC();

		glCreateBuffers(1, &m_RendererID);
		glNamedBufferData(m_RendererID, size, nullptr, GL_DYNAMIC_DRAW);
		m_Count = size / static_cast<uint32_t> ((sizeof(uint32_t)));
	}

	void GLIndexBuffer::bind() const
	{
		RB_PROFILE_FUNC();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void GLIndexBuffer::unbind() const
	{
		RB_PROFILE_FUNC();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void GLIndexBuffer::uploadIndexData(const uint32_t* const data, uint32_t size)
	{
		RB_PROFILE_FUNC();

		glNamedBufferData(m_RendererID, size, data, GL_DYNAMIC_DRAW);
		m_Count = size / sizeof(uint32_t);
	}

	GLIndexBuffer::~GLIndexBuffer()
	{
		RB_PROFILE_FUNC();

		glDeleteBuffers(1, &m_RendererID);

	}
}
