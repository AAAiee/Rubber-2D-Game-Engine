#include <Rubber.h>
#include "ParticleSystem.h"

const float ParticleSystem::FIXED_TIME_STAMP = 1.f / 120.f;

ParticleSystem::ParticleSystem(uint32_t maxCountParticles)
{
	m_ParticlePool.resize(maxCountParticles); 
	m_PoolIndex = maxCountParticles - 1;
}

void ParticleSystem::processAllParticles()
{
	for(auto& particle : m_ParticlePool ){
		if (!particle.m_IsActive)
			continue;

		if(particle.m_LifeRemaining <= 0.0f) {
			particle.m_IsActive = false;
			continue;
		}


		particle.m_LifeRemaining -= FIXED_TIME_STAMP;
		particle.m_Position += particle.m_Velocity * FIXED_TIME_STAMP;
		particle.m_Rotation += 0.10f * FIXED_TIME_STAMP;
	}
}

void ParticleSystem::onRender(const Rubber::OrthoCamera& camera) 
{
	Rubber::Renderer2D::beginScene(camera);
	for(auto& particle:m_ParticlePool){
		if(!particle.m_IsActive){
			continue;
		}
		//fade away particles
		float life = particle.m_LifeRemaining / particle.m_LifeTime;

	    glm::vec4 color = glm::mix(particle.m_ColorEnd, particle.m_ColorBegin, life);
		float size = glm::mix(particle.m_SizeEnd, particle.m_SizeBegin, life);

		Rubber::Renderer2D::drawRotatedQuad({ particle.m_Position.x, particle.m_Position.y, 0.0f }, { size, size }, particle.m_Rotation, color);
	}
	Rubber::Renderer2D::endScene();

}

void ParticleSystem::Emit(const ParticleProps& props)
{
	Particle& particle = m_ParticlePool[m_PoolIndex];
	particle.m_IsActive = true;
	particle.m_Position = props.m_Position;
	particle.m_Rotation = Rubber::RandomEngine::Float() * 2.0f * glm::pi<float>();


	//Velocity
	particle.m_Velocity = props.m_Velocity;
	particle.m_Velocity.x += props.m_VelocityVariation.x * (Rubber::RandomEngine::Float() - 0.5f);
	particle.m_Velocity.y += props.m_VelocityVariation.y * (Rubber::RandomEngine::Float() - 0.5f);

	//Color
	particle.m_ColorBegin = props.m_ColorBegin;
	particle.m_ColorEnd = props.m_ColorEnd;

	particle.m_LifeTime = props.m_LifeTime;
	particle.m_LifeRemaining = props.m_LifeTime;
	particle.m_SizeBegin = props.m_SizeBegin + props.m_SizeVariation * (Rubber::RandomEngine::Float() - 0.5f);

	particle.m_SizeEnd = props.m_SizeEnd;

	m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
}
