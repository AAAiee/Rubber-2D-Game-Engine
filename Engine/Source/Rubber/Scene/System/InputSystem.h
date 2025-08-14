#pragma once
#include <entt.hpp>
#include <string>
#include <unordered_map>
#include "Rubber/Scene/System/System.h"


namespace Rubber {

	class InputSystem :public SystemBase {
	public:
		void init(Ref<Scene> scene) override;
		void shutdown() override;
		void onUpdate(const float ts) override;

	private:
		glm::vec2 mousePosToWorldCoord(float mouseX, float mouseY);

	private:
		std::unordered_map<uint16_t, bool> m_KeysDown;
		float m_MouseX = 0.0f;
		float m_MouseY = 0.0f;
	};
}
