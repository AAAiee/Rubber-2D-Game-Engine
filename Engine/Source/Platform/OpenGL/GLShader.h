# pragma once
#include <string>
#include <unordered_map>
#include <type_traits>
#include <functional>
#include <utility>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Rubber/Renderer/Shaders.h"
#include "Rubber/Core/Assert.h"    // RB_CORE_ASSERT()
#include "Rubber/Core/Search.h"    // stringHash, std::equal_to<>

namespace Rubber {

	namespace UniformUploaderDetail {

		template<typename T> struct UniformUploader;  // forward primary

		template<> struct UniformUploader<float> {
			static constexpr void upload(GLint loc, const float v) {
				glUniform1f(loc, v);
			}
		};

		template<> struct UniformUploader<int> {
			static constexpr void upload(GLint loc, const int v) {
				glUniform1i(loc, v);
			}
		};

		template<> struct UniformUploader<glm::vec2> {
			static constexpr void upload(GLint loc, const glm::vec2& v) {
				glUniform2fv(loc, 1, glm::value_ptr(v));
			}
		};

		template<> struct UniformUploader<glm::vec3> {
			static constexpr void upload(GLint loc, const glm::vec3& v) {
				glUniform3fv(loc, 1, glm::value_ptr(v));
			}
		};

		template<> struct UniformUploader<glm::vec4> {
			static constexpr void upload(GLint loc, const glm::vec4& v) {
				glUniform4fv(loc, 1, glm::value_ptr(v));
			}
		};

		template<> struct UniformUploader<glm::ivec2> {
			static constexpr void upload(GLint loc, const glm::ivec2& v) {
				glUniform2iv(loc, 1, glm::value_ptr(v));
			}
		};

		template<> struct UniformUploader<glm::ivec3> {
			static constexpr void upload(GLint loc, const glm::ivec3& v) {
				glUniform3iv(loc, 1, glm::value_ptr(v));
			}
		};

		template<> struct UniformUploader<glm::ivec4> {
			static constexpr void upload(GLint loc, const glm::ivec4& v) {
				glUniform4iv(loc, 1, glm::value_ptr(v));
			}
		};

		template<> struct UniformUploader<glm::mat3> {
			static constexpr void upload(GLint loc, const glm::mat3& m) {
				glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(m));
			}
		};

		template<> struct UniformUploader<glm::mat4> {
			static constexpr void upload(GLint loc, const glm::mat4& m) {
				glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));
			}
		};

		template<> struct UniformUploader<int*>{
			static constexpr void upload(GLint loc, int* const intArray, uint32_t count){ 
				glUniform1iv(loc, count, intArray);
			}
		};

		// Decide whether to pass T by value or by const ref, depending on size:
		template<typename T>
		constexpr bool lessThanPtr = (sizeof(T) <= sizeof(void*));

		template<bool Small, typename T> struct CorrectTypeHelper {};

		template<typename T>
		struct CorrectTypeHelper<true, T> {
			using Type = const T;      // pass by value if small
		};

		template<typename T>
		struct CorrectTypeHelper<false, T> {
			using Type = const T&;     // pass by const ref if larger than a pointer
		};

		template<typename T>
		using CorrectType_t = typename CorrectTypeHelper<lessThanPtr<T>, T>::Type;

		template<typename T>
		concept isSupportedUniformType = requires {
			std::disjunction_v<
				std::is_same<T, float>, std::is_same<T, int>,
				std::is_same<T, glm::vec2>, std::is_same<T, glm::vec3>,
				std::is_same<T, glm::vec4>,
				std::is_same<T, glm::ivec2>, std::is_same<T, glm::ivec3>,
				std::is_same<T, glm::ivec4>,
				std::is_same<T, glm::mat3>, std::is_same<T, glm::mat4>, std::is_same<T, int*>>;
		};

		template<typename T>
		constexpr auto upload = UniformUploader<T>::upload;
	} // namespace Uniformdetail


	class GLShader : public Shader
	{
	public:
		GLShader(std::string_view name,
			std::string_view vertexSrc,
			std::string_view fragmentSrc);

		GLShader(std::string_view filepath);
		~GLShader();

		// bind/unbind
		void bind()   const final override;
		void unbind() const final override;

		// getters
		inline uint32_t getProgramId()    const          { return m_ShaderID; }
		inline std::string_view getName() const override { return m_Name; }

		// Uniform overrides 

		void setInt(std::string_view name, const int value)                        final override;
		void setIntArray(std::string_view name, int* const values, uint32_t count) final override;
		void setFloat(std::string_view name, const float value)                    final override;
		void setFloat2(std::string_view name, const glm::vec2& vals)               final override;
		void setFloat3(std::string_view name, const glm::vec3& vals)               final override;
		void setFloat4(std::string_view name, const glm::vec4& vals)               final override;
		void setMat3(std::string_view name, const glm::mat3& mat3)                 final override;
		void setMat4(std::string_view name, const glm::mat4& mat4)                 final override;

	private:
		//template helper functions, the implementation for all uniform unloaders
		template <typename T>
		requires UniformUploaderDetail::isSupportedUniformType<T>
		void setUniform(std::string_view name, UniformUploaderDetail::CorrectType_t<T> value)
		{
			bind();  // ensure this shader is active
			GLint loc = getUniformLocation(name);
			UniformUploaderDetail::upload<T>(loc, value);
		}

		// version to upload an array of data
		template<typename T>
		requires requires{std::is_pointer_v<T>;}
		void setUniformnv(std::string_view name, T const values, uint32_t count)
		{
			bind();  // ensure this shader is active
			GLint loc = getUniformLocation(name);
			UniformUploaderDetail::upload<T>(loc, values, count);
		}

		GLint getUniformLocation(std::string_view name) const;
		std::string readShaderFile(std::string_view filepath);
		std::unordered_map<GLint, std::string> preProcess(std::string_view fileContent);
		void compile(const std::unordered_map<GLint, std::string>& shaderSrcMap);

	private:
		uint32_t m_ShaderID = 0;
		std::string m_Name;  // owning name of this shader
		mutable std::unordered_map<std::string, GLint, stringHash, std::equal_to<>> m_UniformCache;
	};

} // namespace Rubber
