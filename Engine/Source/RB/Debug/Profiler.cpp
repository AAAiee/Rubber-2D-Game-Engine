#include <pch.h>
#include "Profiler.h"


namespace RB {
	void Instrumentor::beginSession(const char* name, const char* outputFilePath /*= "Profiler/result.json"*/)
	{
		m_Output.open(outputFilePath);
		RB_CORE_ASSERT(m_Output.is_open(), "Failed to open file!");
		writeHeader();
		// currently it only holds a name( a static literal) so just created on stack
		m_CurSession = ProfileCurrentSession(name);
	}

	void Instrumentor::endSession()
	{
		writeFooter();
		m_Output.close();
		m_ProfileCount = 0ui32;
	}

	void Instrumentor::writeHeader()
	{
		m_Output << "{\"otherData\": {}, \"traceEvents\":[\n";
		m_Output.flush();
	}

	void Instrumentor::writeProfile(const ProfileResult& result)
	{
		if (m_ProfileCount++ > 0) {
			m_Output << ",\n";
		}

		m_Output << "{\n";
		m_Output << "\"cat\":\"function\",\n";
		m_Output << "\"dur\":" <<result.m_End - result.m_Start  << ",\n";
		m_Output << "\"name\":" << "\"" << result.m_Name <<"\"" << ",\n";
		m_Output << "\"ph\":\"X\",\n";
		m_Output << "\"pid\": 0,\n";
		m_Output << "\"tid\":" << result.m_ThreadID << ",\n";
		m_Output << "\"ts\":" << result.m_Start << "\n";
		m_Output << "}";

		m_Output.flush();
	}

	void Instrumentor::writeFooter()
	{
		m_Output << "]}\n";
		m_Output.flush();
	}


	/*||||||||||||||||||||||||||||||||AUTOPROFILE||||||||||||||||||||||||||||||||||||||||||||||*/

	AutoProfile::AutoProfile(const char* name)
		:m_Name(name)
	{
		using namespace std::chrono;
		m_Start = steady_clock::now();
	}

	AutoProfile::~AutoProfile()
	{
		if (!m_Stopped) stop();
	}

	void AutoProfile::stop()
	{
		using namespace std::chrono;
		time_point<steady_clock> endPoint = steady_clock::now();
		m_Stopped = true;

		uint64_t start = time_point_cast<microseconds>(m_Start).time_since_epoch().count();
		uint64_t end = time_point_cast<microseconds>(endPoint).time_since_epoch().count();

		static thread_local uint64_t lastTimeStamp;
		if (start == lastTimeStamp){
			start += 3;
		}
		lastTimeStamp = start;
		

		uint32_t threadID = static_cast<uint32_t> (std::hash<std::thread::id>{} (std::this_thread::get_id()));
		Instrumentor::getInstance().writeProfile({ threadID, start, end, m_Name.data()});
	}

}