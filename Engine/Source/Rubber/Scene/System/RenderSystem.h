#pragma  once
#include <entt.hpp>
#include <Rubber/Scene/Utili/Component.h>

namespace Rubber{
	class Renderer2D;

	struct RendererSystem{

		RendererSystem() = default;
		using DrawGroup = decltype(
			std::declval<entt::registry&>()
			.group<TransformComponent, SpriteComponent>()
			);
		
		void init(entt::registry& regisry);

		void shutdown();

		void onUpdate(const float ts);

		DrawGroup m_Group;
		entt::registry* m_Registry = nullptr;
	};


}