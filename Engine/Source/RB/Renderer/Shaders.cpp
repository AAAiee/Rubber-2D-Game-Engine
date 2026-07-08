#include <pch.h>
#include "Shaders.h"
#include "Platform/OpenGL/GLShader.h"
#include "RB/Renderer/Renderer.h"


namespace RB {

	Scope<RB::Shader> Shader::create(std::string_view filepath)
	{
		switch (Renderer::getAPI()) {
		case RendererAPI::API::NONE:   RB_CORE_ASSERT(false, "NONE-API type is not supported");
		case RendererAPI::API::OpenGL: return makeScope<GLShader>(filepath);
		}

		RB_CORE_ASSERT(false, "The renderer API type is not supported");
		return nullptr;
	}

	Scope<RB::Shader> Shader::create(std::string_view name, std::string_view vertexSrc, std::string_view fragmentSrc)
	{
		switch (Renderer::getAPI()) {
		case RendererAPI::API::NONE:   RB_CORE_ASSERT(false, "NONE-API type is not supported");
		case RendererAPI::API::OpenGL: return makeScope<GLShader>(name, vertexSrc, fragmentSrc);
		}
		RB_CORE_ASSERT(false, "The renderer API type is not supported");
		return nullptr;
	}

	/********************************ShaderLib****************************************/

	void ShaderLib::add(std::string_view name, const Ref<Shader>& shader){
		RB_CORE_ASSERT(!exists(name), "Shader already exists !");
		this->m_AllShaders.emplace(name, shader);
	}

	void ShaderLib::add(const Ref<Shader>& shader)
	{
		std::string_view shaderName = shader->getName();
		add(shaderName, shader);
	}

	void ShaderLib::load(std::string_view path)
	{
		std::filesystem::path curPath(path);
		std::string name = curPath.stem().string();
		RB_CORE_ASSERT(!exists(name), "Trying to override an existing shader is not allowed!");
		Ref<Shader> shader = Shader::create(path);
		this->m_AllShaders.emplace(name, std::move(shader));
	}

    const Ref<RB::Shader> ShaderLib::getShader(std::string_view name) const 
    {  
       const auto& it = this->m_AllShaders.find(name);  
       if (it != this->m_AllShaders.cend()) {  
           return it->second;  
       }  
       RB_CORE_ASSERT(false, "Shader not found!");  
       return nullptr;  
    }

	bool ShaderLib::exists(std::string_view name) const {
		if (this->m_AllShaders.find(name) != this->m_AllShaders.end()) {
			return true;
		}
		return false;
	} 
}
