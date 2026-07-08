#pragma once
#include "RB/Utility/Timer/AnimationTimer.h"
#include "RB/Utility/Utility.h"
#include "RB/Resources/Asset.h"
#include <glm/glm.hpp>

namespace RB {
	struct AnimationClipConfig;

	struct AnimationSpec {

		enum class AnchorMode: uint8_t {
			CENTERED,
			BOTTOMCENTERED
		};

		AnimationSpec() = default;
		AnimationSpec(const AnimationClipConfig& config);

		void setSpec(const AnimationClipConfig& config);

		void setRunOnce(bool flag) {
			m_IsLoop = flag;
		}

		void setFrameCount(uint32_t frameCount) {
			m_FrameCount = frameCount;
		}

		void oppositeDirection(bool flag) {
			m_IsOpposizeDireciton = flag;
		}

		void setAnchorMode(AnchorMode mode) {
			m_AnchorMode = mode;
		}

		void setOnfinished(std::function<void()> function) {
			m_OnFinished = std::move(function);
		}

		AnimationTimer m_Timer;
		bool m_IsLoop = true;
		uint32_t m_FrameIndex = 0;
		AssetHandle m_ActiveAnimationClip;
		uint32_t m_FrameCount = 0;
		bool m_IsOpposizeDireciton = false;
		AnchorMode m_AnchorMode = AnchorMode::CENTERED;
		std::function<void()> m_OnFinished;
	};

	struct AnimationClipConfig {
		std::function<void()> onFinished = nullptr;
		AssetHandle clipHandle;
		float frameDuration = 1.0f;
		bool loop = true;
		bool isFlipped = false;
		AnimationSpec::AnchorMode anchorMode = AnimationSpec::AnchorMode::CENTERED;
		AnimationClipConfig() = default;
	};
}