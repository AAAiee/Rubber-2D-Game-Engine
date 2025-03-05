#pragma once
#include  <Rubber/Core.h>
#include  <spdlog/spdlog.h>
#include  <spdlog/sinks/stdout_color_sinks.h>

namespace Rubber
{
	/*
	* The Logging System for the engine, it uses the spdlog library.
	* The s_Corelogger points to the logger for the engine
	* the s_ClientLogger points to the logger for the application.
	* both are initialized in the Init() function.
	*/
	class RB_API Logger
	{
	public:
	    Logger& operator =(const Logger&) = delete; 
		Logger(const Logger&) = delete;
		static void Init();
		static std::shared_ptr<spdlog::logger>  GetCoreLogger();
		static std::shared_ptr<spdlog::logger> GetClientLogger();

	private:
		Logger() = default;
		static Logger& Get();
	    std::shared_ptr<spdlog::logger>  s_CoreLogger;
		std::shared_ptr<spdlog::logger>  s_ClientLogger;
	};
}

// Define Log macros for the engine.
#define RB_TRACE(...)              ::Rubber::Logger::GetCoreLogger()->trace(__VA_ARGS__);
#define RB_INFO(...)		       ::Rubber::Logger::GetCoreLogger()->info(__VA_ARGS__);
#define RB_WARN(...)               ::Rubber::Logger::GetCoreLogger()->warn(__VA_ARGS__);
#define RB_ERROR(...)		       ::Rubber::Logger::GetCoreLogger()->error( __VA_ARGS__ );
#define RB_CRITICAL(...)           ::Rubber::Logger::GetCoreLogger()->critical( __VA_ARGS__ );

// Define Log macros for the Application.
#define TRACE(...)                 ::Rubber::Logger::GetClientLogger()->trace( __VA_ARGS__ );
#define INFO(...)		           ::Rubber::Logger::GetClienTLogger()->info( __VA_ARGS__ );
#define WARN(...)                  ::Rubber::Logger::GetClientLogger()->warn( __VA_ARGS__ );
#define ERROR(...)		           ::Rubber::Logger::GetClientLogger()->error( __VA_ARGS__ );
#define CRITICAL(...)              ::Rubber::Logger::GetClientLogger()->critical( __VA_ARGS__);
