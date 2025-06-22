#pragma  once
#include <cstdint>
#include <string>
#include <Rubber/Core/Core.h>
namespace Rubber{

	class Texture {
	public:
		virtual ~Texture() {};
		virtual void bind(uint16_t slot) const = 0;
		virtual void unBind() const = 0;
		virtual uint32_t getTextureID() const = 0;
		virtual int32_t getWidth() const = 0;
		virtual int32_t getHeight() const = 0;
	};


	class Texture2D: public Texture{
	public: 
		virtual ~Texture2D() {};
		virtual void setData(void* data, uint32_t size) = 0;
		static Ref<Texture2D> create(const std::string& texturePath);
		static Ref<Texture2D> create(uint16_t width, uint16_t heigh);
		virtual bool operator==(const Texture& texture)  = 0;
	};



}