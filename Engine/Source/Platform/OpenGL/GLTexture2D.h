#pragma once  
#include "Rubber/Renderer/Texture.h"  
#include <glad/glad.h>

namespace Rubber {  

class GLTexture2D: public Texture2D {  

public:  
	GLTexture2D(uint16_t width, uint16_t height);
	GLTexture2D(const std::string& path);  
	virtual ~GLTexture2D();  
	virtual void bind(uint16_t slot) const final override; 
	virtual void unBind() const final override;  
	virtual void setData(void* data, uint32_t size) final override;
	inline virtual uint32_t getTextureID() const final override { return m_RendererID; }
	virtual inline int32_t getWidth() const  
	{  
		return this->m_Width;
	};  

	virtual inline int32_t getHeight() const {  
		return this->m_Height;  
	};   


private:  
	uint32_t  m_RendererID;  
	int32_t m_Height;  
	int32_t m_Width;  
	std::string m_Path;  
	int32_t m_Channels;  
	GLint m_InternalFormat; 
	GLint m_DataFormat;
};  

}