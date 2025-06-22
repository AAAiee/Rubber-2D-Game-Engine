#pragma once
#include<Rubber.h>
#include <glm/glm.hpp>
#include <imgui.h>

class ExampleLayer : public Rubber::Layer
{
public:
	ExampleLayer()
		:Layer("example"), m_Rotation(0.0f)
	{

	}

	void onAttach(Ref<Rubber::EventManager> em){
		m_Camera = makeScope<Rubber::CameraController>(45.0f, 16.0f / 9.0f, 0.1f, 100.0f, em);
		m_Camera->setPosition({ 0.0f, 0.0f, 5.0f });
		this->m_Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		m_Rotation = 0.0f;

		m_Chessboard =Rubber::Texture2D::create("Asset/texture/chessboard.png");

	}

	void onDetach(Ref<Rubber::EventManager> em){

	}

	void onUpdate() override
	{
		m_Camera->onUpdate();
		Rubber::Renderer2D::resetRendererStat();
		Rubber::RendererCommand::clearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Rubber::RendererCommand::clear();
		Rubber::Renderer2D::beginScene(m_Camera->getCamera());
		for (float i = -5.0f ; i  < 5.0f; i+= 0.5f ){
			for (float j = -5.0f; j < 5.0f; j+= 0.5f){
				glm::vec4 color = { (i + 5.0f) / 10.0f, 0.3f, (j + 5.0f) / 10.0f, 1.0f };
				Rubber::Renderer2D::drawQuad({ i * 2.1f , j * 2.1f  , -0.1f }, { 1.f, 1.0f }, color);
			}
		}
		Rubber::Renderer2D::endScene();
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
	Scope<Rubber::CameraController> m_Camera;
	glm::vec4 m_Color{};
	float m_Rotation;
	Ref<Rubber::Texture2D> m_Chessboard;
}; 


