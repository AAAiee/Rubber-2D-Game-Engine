#include "Rubber/Renderer/VertexArray.h"
#include <vector>

namespace Rubber {
	class GLVertexArray : public VertexArray{
	public:
		
		GLVertexArray();
		virtual ~GLVertexArray() ;
		virtual void bind() const final override;
		virtual void unbind() const final override ;

		virtual void addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) final override ;
		virtual void setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) final override;

		virtual std::vector<Ref<VertexBuffer>>& getVertexBuffers() final override ;
		virtual Ref<IndexBuffer>& getIndexBuffer() final override ;

	private:
		uint32_t m_RendererID;
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
	};
}