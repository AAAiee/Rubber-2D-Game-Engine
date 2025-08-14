#include "EnemyAttack.h"
#include "ECS/GameComponents/GameComponents.h"



void EnemyAttack::onCreate()
{
	auto& enemyEnt = getComponent<ParentEntityComponent>().parent;
	glm::vec3 enemyPos = enemyEnt.getComponent<RB::TransformComponent>().position;

	auto& tsC = getComponent<RB::TransformComponent>();
	tsC.position.x = enemyPos.x;
	tsC.position.y = enemyPos.y;
	tsC.position.z = enemyPos.z ;

	auto& spriteC = getComponent<RB::SpriteComponent>();
	spriteC.color = { 0.0f, 0.0f,0.0f, 0.0f };
	spriteC.localOffset = { 0.0f, 0.5f };

	auto& CollisionC = getComponent<RB::CollisionComponent>();
	CollisionC.enabled = false;
	CollisionC.srcLayer = RB::CollisionLayer::None;
	CollisionC.targetLayer = RB::CollisionLayer::Player;
	CollisionC.size = { 1.0f, 1.0f };
}
