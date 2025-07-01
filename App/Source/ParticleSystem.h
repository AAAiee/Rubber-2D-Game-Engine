#pragma once

const float FIXED_TIME_STAMP = 1.0f / 120.0f;

class Rubber::Camera;

struct ParticleProps{
	glm::vec4 m_ColorBegin{}, m_ColorEnd{};
	glm::vec2 m_Position{};
	glm::vec2 m_Velocity{}, m_VelocityVariation{};

	float m_SizeBegin = 0.0f, m_SizeEnd = 0.0f, m_SizeVariation =0.0f;
	float m_LifeTime = 1.0f;
};

class ParticleSystem{
	
public:
	ParticleSystem() = default;
	ParticleSystem(uint32_t count=1024);

	void processAllParticles();

	void onRender(const Rubber::Camera& camera);

	void Emit(const ParticleProps& props);
	 
private:
	static const float FIXED_TIME_STAMP;
	struct Particle {
		glm::vec2 m_Position{ 0.0f, 0.0f };
		glm::vec2 m_Velocity{ 0.0f, 0.0f };
		glm::vec4 m_ColorBegin{ 0.0f, 0.0f, 0.0f, 0.0f };
		glm::vec4 m_ColorEnd{ 0.0f, 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;
		float m_SizeBegin = 0.0f;
		float m_SizeEnd = 0.0f;
		float m_LifeTime = 1.0f;
		float m_LifeRemaining = 0.0f;
		bool m_IsActive = false;
	};

	Rubber::Vector<Particle> m_ParticlePool;
	uint32_t m_PoolIndex;
};

