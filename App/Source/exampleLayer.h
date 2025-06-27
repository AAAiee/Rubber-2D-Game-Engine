#pragma once
#include<Rubber.h>
#include "ParticleSystem.h"
#include <glm/glm.hpp>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Rubber::Layer
{
public:
	ExampleLayer()
		:Layer("example"), m_Rotation(0.0f)
	{

	}

	void onAttach(Ref<Rubber::EventManager> em) {
		m_ParticleSystem = makeRef<ParticleSystem>(1024);
		m_Camera = makeScope<Rubber::OrthoCameraController>(16.0f / 9.0f, em, true);
		m_Camera->setPosition({ 0.0f, 0.0f, 0.0f });
		this->m_Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		m_Rotation = 0.0f;
		m_Chessboard = Rubber::Texture2D::create("Asset/texture/chessboard.png");


		// Init here
		m_Particle.m_ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
		m_Particle.m_ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
		m_Particle.m_SizeBegin = 0.5f, m_Particle.m_SizeVariation = 0.3f, m_Particle.m_SizeEnd = 0.0f;
		m_Particle.m_LifeTime = 2.0f;
		m_Particle.m_Velocity = { 0.0f, 0.0f };
		m_Particle.m_VelocityVariation = { 3.0f, 1.0f };
		m_Particle.m_Position = { 0.0f, 0.0f };
	}

	void onDetach(Ref<Rubber::EventManager> em){

	}

	void onUpdate() override
	{
		m_Camera->ProcessInputs();
		m_ParticleSystem->processAllParticles();

		Rubber::Renderer2D::resetRendererStat();
		Rubber::RendererCommand::clearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Rubber::RendererCommand::clear();
		Rubber::Renderer2D::beginScene(m_Camera->getCamera());

		for (float i = -5.0f ; i  < 5.0f; i+= 0.5f ){
			for (float j = -5.0f; j < 5.0f; j+= 0.5f){
				glm::vec4 color = { (i + 5.0f) / 10.0f, 0.3f, (j + 5.0f) / 10.0f, 1.0f };
				Rubber::Renderer2D::drawQuad({i, j, -0.2f }, { 0.45f, 0.45f }, color);
			}
		}
		Rubber::Renderer2D::endScene();


		if (Rubber::Input::isMouseButtonPressed(RB_MOUSE_BUTTON_LEFT))
		{
			auto [x, y] = Rubber::Input::getMousePosition();
			auto width =(float) Rubber::Application::getWindow().getWidth();
			auto height = (float) Rubber::Application::getWindow().getHeight();

			auto bounds = m_Camera->getBounds();

			auto pos = m_Camera->getPosition();
			x = (x / width) * bounds.getWidth() - bounds.getWidth() * 0.5f;
			y = bounds.getHeight() * 0.5f - (y / height) * bounds.getHeight();
			m_Particle.m_Position = { x + pos.x, y + pos.y };
			for (int i = 0; i < 5; i++)
				m_ParticleSystem->Emit(m_Particle);
		}

		m_ParticleSystem->onRender(m_Camera->getCamera());

	}

	void onImGuiRender() override
	{
		ImGui::Begin("Example Layer");
		ImGui::Text("DrawCount: %d", Rubber::Renderer2D::getDrawCallCount());
		ImGui::Text("QuadCount: %d", Rubber::Renderer2D::getQuadCount());
		ImGui::Text("IndexCount: %d", Rubber::Renderer2D::getIndexCount());
		ImGui::Text("VertexCount: %d", Rubber::Renderer2D::getVertexCount()); 
		ImGui::Text("Frame Time: %f", Rubber::Application::getTimer()->getAverageFrameTime());
		ImGui::Text("Current FPS : %d", Rubber::Application::getTimer()->getFps());

		ImGui::End();
	}

private:
	Scope<Rubber::OrthoCameraController> m_Camera;
	glm::vec4 m_Color{};
	float m_Rotation;
	Ref<Rubber::Texture2D> m_Chessboard;
	Ref<ParticleSystem> m_ParticleSystem;
	ParticleProps m_Particle;
}; 


