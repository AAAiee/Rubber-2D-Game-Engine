#include <pch.h>
#include "GLTexture2D.h"
#include <glad/glad.h>
#include <stb_image.h>


namespace Rubber{
	GLTexture2D::GLTexture2D(const std::string& path)
		:m_Path(path)
	{
		stbi_set_flip_vertically_on_load(true);

		unsigned char* data =stbi_load(path.c_str(), &this->m_Width, &this->m_Height, &this->m_Channels, 0);
		if (!data){
			RB_CORE_ASSERT(false, "Fail to load the texture image!");
		}

		// Internal format can be different compared to the data format
		GLint internalFormat = 0, dataFormat = 0;
		if (m_Channels == 3){
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}else if ( m_Channels == 4){
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}

		RB_CORE_ASSERT(internalFormat && dataFormat, "only support rgba and rgb for now");

		m_DataFormat = dataFormat;
		m_InternalFormat = internalFormat;

		// OpenGL 3.3 legacy code
		//glGenTextures(1, &this->m_RendererID);
		//glBindTexture(GL_TEXTURE_2D, this->m_RendererID);
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, this->m_Width, this->m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, this->m_Width, this->m_Height, dataFormat, GL_UNSIGNED_BYTE, data);
		glGenerateTextureMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
	}

	GLTexture2D::GLTexture2D(uint16_t width, uint16_t height)
		:m_Width(width), m_Height(height)
	{
		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, this->m_Width, this->m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}

	GLTexture2D::~GLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void GLTexture2D::bind(uint16_t slot) const
	{
		// 3.3 glActiveTexture(GL_TEXTURE0 + slot);
		glBindTextureUnit(slot, this->m_RendererID);
	}

	void GLTexture2D::unBind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void GLTexture2D::setData(void* data, uint32_t size)
	{
		uint16_t bytePerPixel = m_DataFormat == GL_RGB ? 3 : 4;
		RB_CORE_ASSERT(size == bytePerPixel * m_Width * m_Height, "Size of data must be the size of the full texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

}


