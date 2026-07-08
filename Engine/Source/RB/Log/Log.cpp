#include <pch.h>
namespace RB
{
	void RB::Logger::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		Get().s_ClientLogger = spdlog::stdout_color_mt("Client");
		Get().s_ClientLogger->set_level(spdlog::level::trace);
		Get().s_CoreLogger = spdlog::stdout_color_mt("RB");
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

	Logger& RB::Logger::Get()
	{
		static Logger Instance;
		return Instance;
	}
}
