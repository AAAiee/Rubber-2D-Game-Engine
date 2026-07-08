#pragma once
#include <stdint.h>
#include <memory>

/**
* 
* @file Buffers.h
* @brief Buffers.h is a header file that contains the definition of the VertexBuffer and IndexBuffer classes.
* each class is an abstract base class that defines the interface for creating and managing vertex and index buffers.
* The static create method for each buffer are called during run time to 
* choose the specific API implementation of the buffer.
*/ 
namespace RB {
	class BufferLayout;
	class VertexBuffer {
	public:
		virtual ~VertexBuffer() {} ;
		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		virtual void setLayout(const BufferLayout& layout) = 0;
		virtual void uploadVertexData(const void* const data, uint32_t size) = 0;
		virtual const BufferLayout& getLayout() const = 0;
		static Scope<VertexBuffer> create(float* vertices, uint32_t size);
		static Scope<VertexBuffer> create(uint32_t size);
	};


	class IndexBuffer {
	public:
		virtual ~IndexBuffer() {};
		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		virtual uint32_t getCount() const = 0;
		virtual void uploadIndexData(const uint32_t* const data, uint32_t size) = 0;
		static Scope<IndexBuffer> create(uint32_t* indices, uint32_t size);
		static Scope<IndexBuffer> create(uint32_t size);
	};

}
