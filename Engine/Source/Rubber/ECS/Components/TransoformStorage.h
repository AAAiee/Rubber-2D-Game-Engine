#pragma once
#include "Rubber/ECS/EcsCommonHeaders.h"
#include "ComponentTemplate.h"


namespace Rubber{
	// TransformStorage.h -------------------------------------------
	class TransformStorage : public ComponentBase<
		glm::vec3,        // Position
		glm::quat,        // Rotation
		glm::vec3         // Scale
	> {
		using Base = ComponentBase<glm::vec3, glm::quat, glm::vec3>;

	public:
		/// helper aliases ¨C easier to use in client code
		using Position = glm::vec3;
		using Rotation = glm::quat;
		using Scale = glm::vec3;

		/// high-level add
		void add(Entity e,
			const Position& p,
			const Rotation& r = Rotation{ 1.0f, 0.0f, 0.0f, 0.0f },
			const Scale& s = Scale(1.0f) ) {
			Base::addComponent(e, p, r, s);
		}

		/// getters that hide template gymnastics
		Position& position(Entity e) { return Base::get<Position>(e); }
		Rotation& rotation(Entity e) { return Base::get<Rotation>(e); }
		Scale& scale(Entity e) { return Base::get<Scale>(e); }
		bool contains(Entity e){
			return (Base::lookup(e).index != ID::invalidId);
		}

		glm::mat4 localMatrix(Entity e) {
			return glm::translate(glm::mat4(1.0f), position(e))
				* glm::mat4_cast(rotation(e))
				* glm::scale(glm::mat4(1.0f), scale(e));
		}
	};




}