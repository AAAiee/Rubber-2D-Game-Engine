#include "pch.h"
#include <GLFW/glfw3.h>
#include "GameLoopTimer.h"

namespace Rubber{
	GameLoopTimer::GameLoopTimer(int targetFps /*= 60*/)
		: m_FrameTargetDuration(1.0 / targetFps),
		m_Frames(0),
		m_CurrentFps(targetFps),
		m_SleepError(0.0),
		m_FrameTimeCount(0),
		m_FrameTimeIndex(0),
		m_IsInitialized(false)
		
	{
		// TODO:this is windows only, may need to be fixed in the future
		timeBeginPeriod(1);
		std::fill(this->m_FrameTimes.begin(), this->m_FrameTimes.end(), this->m_FrameTargetDuration.count());
	}

	GameLoopTimer::~GameLoopTimer()
	{
		timeEndPeriod(1);
	}

	int GameLoopTimer::getFps() const
	{
		return this->m_CurrentFps;
	}

	void GameLoopTimer::startFrame()
	{
		// update the frame Start time
		this->m_FrameStartTime = Clock::now();
		this->m_Frames++; 
		
		if (!this->m_IsInitialized){
			this->m_LastFpsUpdate = this->m_FrameStartTime;
			this->m_IsInitialized = true;
		}

		Duration fpsUpdateElapsedTime = this->m_FrameStartTime - m_LastFpsUpdate;
		if (fpsUpdateElapsedTime.count() >= GameLoopTimer::FPS_UPDATE_INTERVAL){
			// plus 0.5 to avoid truncating 
			this->m_CurrentFps = static_cast<int> (this->m_Frames / fpsUpdateElapsedTime.count() + 0.5);
			this->m_Frames = 0;
			this->m_LastFpsUpdate = m_FrameStartTime;
		}
	}

	// TODO:MAYBE FIX THE SLEEP ERROR ADJUSTMENT
	void GameLoopTimer::waitForFrameEnd()
	{
		TimePoint preSleepTime = Clock::now();
		auto targetFrameEndTime = this->m_FrameStartTime + this->m_FrameTargetDuration - m_SleepError; 

		if (preSleepTime < targetFrameEndTime) {

			if (targetFrameEndTime - preSleepTime > Duration(0.001)) {
				Duration actualSleepDuration = (targetFrameEndTime - preSleepTime) * 0.85;
				std::this_thread::sleep_until(preSleepTime + actualSleepDuration);
			}

			//busy wait for  the rest of the time
			while (Clock::now() < targetFrameEndTime) {
				std::this_thread::yield(); // yield to other threads if needed
			}


			Duration overshoot = Clock::now() - targetFrameEndTime;
			this->m_SleepError = overshoot > Duration(0.0) ? (overshoot) * 0.5 : Duration(0.0);

		} else{
			this->m_SleepError = Duration(0.0); // reset sleep error if we are behind
		}

		this->m_FrameTimes[this->m_FrameTimeIndex] = getElapsedTime();
		this->m_FrameTimeIndex = (this->m_FrameTimeIndex + 1) % GameLoopTimer::MAX_FRAME_HISTORY;
		this->m_FrameTimeCount = min(m_FrameTimeCount + 1, GameLoopTimer::MAX_FRAME_HISTORY);
	}

	double GameLoopTimer::getElapsedTime() const
	{
		return Duration(Clock::now() - this->m_FrameStartTime).count();
	}

	double GameLoopTimer::getAverageFrameTime() const
	{
		if (this->m_FrameTimeCount == 0) return this->m_FrameTargetDuration.count();
		return std::accumulate(this->m_FrameTimes.begin(), this->m_FrameTimes.begin() + this->m_FrameTimeCount, 0.0) / this->m_FrameTimeCount;
	}

	double GameLoopTimer::getDeltaTime() const
	{
		int last = (this->m_FrameTimeIndex + MAX_FRAME_HISTORY - 1) % MAX_FRAME_HISTORY;
		return this->m_FrameTimes[last];
	}

}
