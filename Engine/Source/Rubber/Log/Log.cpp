#include <pch.h>
#include <glm/glm.hpp>


namespace fmt {

	template<>
	struct formatter<glm::vec3> : formatter<std::string>
	{
		auto format(glm::vec3 my, format_context& ctx) const -> decltype(ctx.out())
		{
			return fmt::format_to(ctx.out(), "[glm::vec3 x={} y={} z={}]", my.x, my.y, my.z);
		}
	};


	template<>
	struct formatter<glm::vec4> : formatter<std::string>
	{
		auto format(glm::vec4 my, format_context& ctx) const -> decltype(ctx.out())
		{
			return fmt::format_to(ctx.out(), "[glm::vec4 x={} y={} z={} w={}]", my.x, my.y, my.z, my.w);
		}
	};
}


namespace Rubber
{
	void Rubber::Logger::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		Get().s_ClientLogger = spdlog::stdout_color_mt("Client");
		Get().s_ClientLogger->set_level(spdlog::level::trace);
		Get().s_CoreLogger = spdlog::stdout_color_mt("Rubber");
		Get().s_CoreLogger->set_level(spdlog::level::trace);
	}

	std::shared_ptr<spdlog::logger>& Logger::GetCoreLogger()
	{
		return Get().s_CoreLogger;
	
	}

	std::shared_ptr<spdlog::logger>& Logger::GetClientLogger()
	{
		return Get().s_ClientLogger;
	}

	Logger& Rubber::Logger::Get()
	{
		static Logger Instance;
		return Instance;
	}
}
