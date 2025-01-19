#include "Log.h"
#include <iostream>
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
