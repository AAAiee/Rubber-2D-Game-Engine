#pragma  once
#include <functional>

namespace Rubber {
	class AnimationTimer {
	private:
		using onTimeOutFunc = std::function<void()>;
	public:
		AnimationTimer() = default;
		~AnimationTimer() = default;

		template<typename FuncType> 
		explicit AnimationTimer(const float durationPerRun, FuncType&& func ,bool loop = false)
			:m_IsOneTimeUse(!loop), m_DurationPerRun(durationPerRun), m_OnTimeOutCallback(std::forward<FuncType>(func)){ }

		void Pause();

		void resume();
		
		void restart();

		void setWaitTime( float durationPerRun);

		void onUpdate( float ts);

		void runOnlyOnce(bool flag);
		
		void setOnTimeOut(onTimeOutFunc&& timeout) {
			m_OnTimeOutCallback = std::move(timeout);
		}

	private:
		bool m_IsPaused = false;
		bool m_IsOneTimeUse = true;
		bool m_IsSingleRunEnded = false;
		float m_TimePassed = 0.0f;
		float m_DurationPerRun = 1.0f;
		onTimeOutFunc m_OnTimeOutCallback = nullptr;
	};


}