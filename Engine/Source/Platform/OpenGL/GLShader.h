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

	namespace Uniformdetail {

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
			using Type = const T&;     // pass by const ref if larger
		};

		template<typename T>
		using CorrectType_t = typename CorrectTypeHelper<lessThanPtr<T>, T>::Type;

		// Which types are allowed in setUniform<бн>?
		template<typename T>
		constexpr bool isSupportedType = std::disjunction_v<
			std::is_same<T, float>, std::is_same<T, int>,
			std::is_same<T, glm::vec2>, std::is_same<T, glm::vec3>,
			std::is_same<T, glm::vec4>,
			std::is_same<T, glm::ivec2>, std::is_same<T, glm::ivec3>,
			std::is_same<T, glm::ivec4>,
			std::is_same<T, glm::mat3>, std::is_same<T, glm::mat4>>;

		// A shorthand for calling UniformUploader<T>::upload
		template<typename T>
		constexpr auto upload = UniformUploader<T>::upload;
	} // namespace Uniformdetail


	class GLShader : public Shader
	{
	public:
		//  a) Constructor from (name, vertexSrc, fragmentSrc):
		GLShader(std::string_view name,
			std::string_view vertexSrc,
			std::string_view fragmentSrc);

		//  b) Constructor from a single file with multiple "#type" blocks:
		GLShader(std::string_view filepath);
		~GLShader();

		// bind/unbind
		void bind()   const final override;
		void unbind() const final override;

		// getters
		inline uint32_t getProgramId() const { return m_ShaderID; }
		inline std::string_view getName() const override { return m_Name; }

		// Uniform overrides (using string_view):
		void setInt(std::string_view name, int value)           final override;
		void setFloat(std::string_view name, float value)         final override;
		void setFloat2(std::string_view name, const glm::vec2& v)  final override;
		void setFloat3(std::string_view name, const glm::vec3& v)  final override;
		void setFloat4(std::string_view name, const glm::vec4& v)  final override;
		void setMat3(std::string_view name, const glm::mat3& m)  final override;
		void setMat4(std::string_view name, const glm::mat4& m)  final override;

	private:
		// templated implementation helper's that Uniformdetail is known:
		template<typename T, typename = std::enable_if_t<Uniformdetail::isSupportedType<T>>>
		void setUniform(std::string_view name, Uniformdetail::CorrectType_t<T> value)
		{
			bind();  // ensure this shader is active
			GLint loc = getUniformLocation(name);
			Uniformdetail::upload<T>(loc, value);
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
