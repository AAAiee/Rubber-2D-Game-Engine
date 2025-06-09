#include <pch.h>
#include "Rubber/Renderer/BufferLayout.h"

namespace Rubber {

	// return the underlying size of a given type
	uint32_t Rubber::getShaderTypeSize(ShaderType type) {
		switch (type)
		{
			case ShaderType::Float:   return 4;
			case ShaderType::Float2:  return 4 * 2;
			case ShaderType::Float3:  return 4 * 3;
			case ShaderType::Float4:  return 4 * 4;
			case ShaderType::Mat2:    return 4 * 2 * 2;
			case ShaderType::Mat3:    return 4 * 3 * 3;
			case ShaderType::Mat4:    return 4 * 4 * 4;
			case ShaderType::Int:     return 4;
			case ShaderType::Int2:    return 4 * 2;
			case ShaderType::Int3:    return 4 * 3;
			case ShaderType::Int4:    return 4 * 4;
			case ShaderType::Uint:    return 4;
			case ShaderType::Uint2:   return 4 * 2;
			case ShaderType::Uint3:   return 4 * 3 ;
			case ShaderType::Uint4:   return 4 * 4;
		}

		RB_CORE_ASSERT(false, "Unknown Shader Type");
		return 0;
	}

	
	// takes a initiaizer_list and calculate offset and strides
	BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& layout)
		:m_ElementBuffer(layout), m_Stride(0)
	{
		int cnt_Offset = 0;
		for (int i = 0; i < m_ElementBuffer.size(); ++i) {
			this->m_ElementBuffer[i].offset = cnt_Offset;
			cnt_Offset += getShaderTypeSize(m_ElementBuffer[i].type);
			this->m_Stride = cnt_Offset;
		}
	}

	// return the count of each component
	uint32_t BufferElement::getComponentCount() const
	{
		switch (this->type) {
			case ShaderType::Float:   return 1;
			case ShaderType::Float2:  return 2;
			case ShaderType::Float3:  return 3;
			case ShaderType::Float4:  return 4;
			case ShaderType::Mat2:    return 2 * 2;
			case ShaderType::Mat3:    return 3 * 3;
			case ShaderType::Mat4:    return 4 * 4;
			case ShaderType::Int:     return 1;
			case ShaderType::Int2:    return 2;
			case ShaderType::Int3:    return 3;
			case ShaderType::Int4:    return 4;
			case ShaderType::Uint:    return 1;
			case ShaderType::Uint2:   return 2;
			case ShaderType::Uint3:   return 3;
			case ShaderType::Uint4:   return 4;
		}

		RB_CORE_ASSERT(false, "ShaderType is not supported!");
		return 0;
	}

}
