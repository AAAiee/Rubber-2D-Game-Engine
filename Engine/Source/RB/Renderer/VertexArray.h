#pragma once
#include <memory>
#include <vector>

namespace RB {
	class IndexBuffer;
	class VertexBuffer;

	class VertexArray {
	public:
		virtual ~VertexArray() {};
		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual void addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;

		virtual Vector<Ref<VertexBuffer>>& getVertexBuffers() = 0;
		virtual Ref<IndexBuffer>& getIndexBuffer() = 0;
		static Ref<VertexArray> create();
	};
}