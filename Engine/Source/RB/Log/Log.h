#pragma once
#include  <RB/Core/Core.h>
#include  <spdlog/spdlog.h>
#include  <spdlog/sinks/stdout_color_sinks.h>

namespace RB
{
	/*
	* The Logging System for the engine, it uses the spdlog library.
	* The s_Corelogger points to the logger for the engine
	* the s_ClientLogger points to the logger for the application.
	* both are initialized in the Init() function.
	*/
	class Logger
	{
	public:
	    Logger& operator =(const Logger&) = delete; 
		Logger(const Logger&) = delete;
		static void Init();
		static std::shared_ptr<spdlog::logger>&  GetCoreLogger();
		static std::shared_ptr<spdlog::logger>& GetClientLogger();

	private:
		Logger() = default;
		static Logger& Get();
	    std::shared_ptr<spdlog::logger>  s_CoreLogger;
		std::shared_ptr<spdlog::logger>  s_ClientLogger;
	};
}

// Define Log macros for the engine.
#define RB_TRACE(...)              ::RB::Logger::GetCoreLogger()->trace(__VA_ARGS__);
#define RB_INFO(...)		       ::RB::Logger::GetCoreLogger()->info(__VA_ARGS__);
#define RB_WARN(...)               ::RB::Logger::GetCoreLogger()->warn(__VA_ARGS__);
#define RB_ERROR(...)		       ::RB::Logger::GetCoreLogger()->error( __VA_ARGS__ );
#define RB_CRITICAL(...)           ::RB::Logger::GetCoreLogger()->critical( __VA_ARGS__ );

// Define Log macros for the Application.
#define APP_TRACE(...)                 ::RB::Logger::GetClientLogger()->trace( __VA_ARGS__ );
#define APP_INFO(...)		           ::RB::Logger::GetClientLogger()->info( __VA_ARGS__ );
#define APP_WARN(...)                  ::RB::Logger::GetClientLogger()->warn( __VA_ARGS__ );
#define APP_ERROR(...)		           ::RB::Logger::GetClientLogger()->error( __VA_ARGS__ );
#define APP_CRITICAL(...)              ::RB::Logger::GetClientLogger()->critical( __VA_ARGS__);
