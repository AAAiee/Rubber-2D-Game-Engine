#include<Rubber.h>
#include <glm/glm.hpp>
#include <imgui.h>

class ExampleLayer : public Rubber::Layer
{
public:
	ExampleLayer()
		:Layer("example")
	{
		m_Camera = makeScope<Rubber::CameraController>(45.0f, 16.0f / 9.0f, 0.1f, 100.0f);
		m_Camera->setPosition({ 0.0f, 0.0f, 5.0f });
		this->m_Color = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
	}

	void onUpdate() override
	{
		m_Camera->onUpdate();
		Rubber::RendererCommand::clearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Rubber::RendererCommand::clear();

		Rubber::Renderer2D::beginScene(m_Camera->getCamera());
		Rubber::Renderer2D::drawQuad({ 0.8f, 0.2f, -0.5f }, { 1.0f, 2.0f, 1.0f }, m_Color);
		Rubber::Renderer2D::drawQuad({ 0.3f, 0.5f, -0.5f }, { 1.0f, 2.0f, 1.0f }, m_Color);
		Rubber::Renderer2D::drawQuad({ 0.2f, 0.3f, -0.8f }, { 1.0f, 2.0f, 1.0f }, m_Color);
		
		Rubber::Renderer2D::endScene();
	}

	void onImGuiRender() override
	{
		ImGui::Begin("Example Layer");
		ImGui::SliderFloat4("Color", &m_Color.x, 0.1f, 1.0f);
		ImGui::End();
	}

	void onEvent(Rubber::Event& event) override
	{
		m_Camera->onEvent(event);
	}


private:
	Scope<Rubber::CameraController> m_Camera;
	glm::vec4 m_Color;
}; 


