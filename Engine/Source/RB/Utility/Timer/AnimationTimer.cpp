#include <pch.h>
#include "AnimationTimer.h"


namespace RB {

	void AnimationTimer::Pause()
	{
		m_IsPaused = true;
	}

	void AnimationTimer::resume()
	{
		m_IsPaused = false;
	}

	void AnimationTimer::restart()
	{
		m_TimePassed = 0.0f;
		m_IsSingleRunEnded = false;
	}

	void AnimationTimer::setWaitTime(const float durationPerRun)
	{
		m_DurationPerRun = durationPerRun;
	}

	void AnimationTimer::onUpdate(const float ts)
	{
		if (m_IsPaused) return;

		m_TimePassed += ts; 
		if (m_TimePassed >= m_DurationPerRun) {
			bool shouldFireCallBack = !m_IsSingleRunEnded || !m_IsOneTimeUse;
			if (shouldFireCallBack && m_OnTimeOutCallback != nullptr) {
				m_OnTimeOutCallback();
			}
			m_IsSingleRunEnded = true;
			m_TimePassed -= m_DurationPerRun;
		}
	}

	void AnimationTimer::runOnlyOnce(bool flag)
	{
		m_IsOneTimeUse = flag;
	}

}

