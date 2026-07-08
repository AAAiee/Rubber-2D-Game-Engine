#pragma once
#include <string>
#include <chrono>
#include <iostream>

namespace RB {

	// currently only store the session name, but will be extended
	struct ProfileCurrentSession{
		const char* name;
	};

	// store the results that will be written in the JSON file
	struct ProfileResult{
		uint32_t m_ThreadID;
		uint64_t m_Start;
		uint64_t m_End;
		std::string m_Name;
	};

	// write all traced events in a JSON file for visualization.
	class Instrumentor{
	public:
		
		void beginSession(const char* name, const char* outputFilePath = "Profiler/result.json");
		void endSession();
		void writeProfile(const ProfileResult& result);

	public:
		static Instrumentor& getInstance() {
			static Instrumentor s_Instance;
			return s_Instance;
		}

	private:
		void writeHeader();
		void writeFooter();

	private:
		uint32_t m_ProfileCount{ 0ui32 };
		std::ofstream m_Output;
		ProfileCurrentSession m_CurSession{""};
	private:
		Instrumentor() {};
	};



	class AutoProfile {
	public:
		AutoProfile(const char* name);
		~AutoProfile();

	private:
		void stop();

	private:
		bool m_Stopped = false;
		std::string_view m_Name;
		std::chrono::time_point<std::chrono::steady_clock> m_Start;
	};
}


#define RB_PROFILE_ENABLED 0
#if RB_PROFILE_ENABLED
	#define RB_PROFILE_SCOPE(name) ::RB::AutoProfile profile##__LINE__(name)
	#define RB_PROFILE_FUNC() RB_PROFILE_SCOPE(__FUNCSIG__) 
	#define RB_PROFILE_BEGIN_SESSION(name,filepath) \
					::RB::Instrumentor::getInstance().beginSession(name,filepath)
	#define RB_PROFILE_END_SESSION() \
					::RB::Instrumentor::getInstance().endSession()
#else
	#define RB_PROFILE_FUNC()
	#define RB_PROFILE_SCOPE(name) 
	#define RB_PROFILE_BEGIN_SESSION(name, filepath)
	#define RB_PROFILE_END_SESSION() 
#endif
