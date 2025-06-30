#pragma once
#include<Rubber.h>
#include <glm/glm.hpp>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>


class EditorLayer : public Rubber::Layer
{
public:
	EditorLayer()
		:Layer("example") 
	{

	}

	void onAttach(const Ref<Rubber::EventManager>& em) override {
		m_EventManager = em;
		m_Camera = makeScope<Rubber::OrthoCameraController>(16.0f / 9.0f, em, true);
		m_Camera->setPosition({ 0.0f, 0.0f, 0.0f });

		m_Chessboard = Rubber::Texture2D::create("Asset/texture/chessboard.png");
		m_SpriteSheet = Rubber::Texture2D::create("Asset/texture/tilemap_packed.png");
		m_Axe = Rubber::SubTexture2D::create(m_SpriteSheet, { 7, 0 }, { 16, 16 }, { 1,1 }); 

		auto width = Rubber::Application::getWindow().getWidth();
		auto height = Rubber::Application::getWindow().getHeight();
		m_FrameBuffer = Rubber::FrameBuffer::create({ width,height,1,false });
	}

	void onDetach(){

	}

	void onUpdate() override
	{
		if (m_IsViewPortFocused) {
			m_Camera->ProcessInputs();
		}

		glm::vec2 frameBufferSizeBefore = { m_FrameBuffer->getSpecification().m_Width, m_FrameBuffer->getSpecification().m_Height };
		if (m_ViewPortDimension.x * m_ViewPortDimension.y != 0.0f && frameBufferSizeBefore != m_ViewPortDimension) {
			m_FrameBuffer->resize((uint32_t)m_ViewPortDimension.x, (uint32_t)m_ViewPortDimension.y); 
		}

		auto frameBufferSpecAfter = m_FrameBuffer->getSpecification();
		m_Camera->updateAspectRatio(frameBufferSpecAfter.m_Width, frameBufferSpecAfter.m_Height);

		

		m_FrameBuffer->bind();
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

		Rubber::Renderer2D::beginScene(m_Camera->getCamera());
		Rubber::Renderer2D::drawQuad({ -1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, m_Axe);
		Rubber::Renderer2D::drawRotatedQuad({ -4.f, 0.0f, 0.0f }, { 2.0f, 2.0f }, glm::radians(-45.0f), m_Axe);
		Rubber::Renderer2D::endScene();
		m_FrameBuffer->unbind();
	}

	void onImGuiRender() override
	{
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static bool dockSpaceOpen = true;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockSpaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
	
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
				ImGui::MenuItem("Padding", NULL, &opt_padding);
				ImGui::Separator();

				if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
				if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
				if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
				if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
				if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
				ImGui::Separator();

				if (ImGui::MenuItem("Close", NULL, false))
					dockSpaceOpen = false;
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::Begin("Setting");
		ImGui::Text("DrawCount: %d", Rubber::Renderer2D::getDrawCallCount());
		ImGui::Text("QuadCount: %d", Rubber::Renderer2D::getQuadCount());
		ImGui::Text("IndexCount: %d", Rubber::Renderer2D::getIndexCount());
		ImGui::Text("VertexCount: %d", Rubber::Renderer2D::getVertexCount());
		ImGui::Text("Frame Time: %f", Rubber::Application::getTimer()->getAverageFrameTime());
		ImGui::Text("Current FPS : %d", Rubber::Application::getTimer()->getFps());
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f,0.0f });
		ImGui::Begin("ViewPort");  

		m_IsViewPortFocused = ImGui::IsWindowFocused();
		m_IsViewPortHovered = ImGui::IsWindowHovered();

		if(!m_IsViewPortFocused){
			m_Camera->unsubscribeAllEvent();
		}else{
			m_Camera->subscribeAllEvent();
		}

		ImVec2 curViewPortSize = ImGui::GetContentRegionAvail();
		m_ViewPortDimension = { curViewPortSize.x, curViewPortSize.y };
		ImGui::Image((void*)m_FrameBuffer->getColorAttachmentID(), *(ImVec2*)&m_ViewPortDimension,{ 0,1 }, {1,0});
		ImGui::End();
		ImGui::PopStyleVar();


		ImGui::End();
	}

private:
	Ref<Rubber::EventManager> m_EventManager;
	Scope<Rubber::OrthoCameraController> m_Camera;
	Ref<Rubber::Texture2D> m_Chessboard;
	Ref<Rubber::Texture2D> m_SpriteSheet;
	Ref<Rubber::FrameBuffer> m_FrameBuffer;
	Ref<Rubber::SubTexture2D> m_Axe; 

private:
	glm::vec2 m_ViewPortDimension{};
	bool m_IsViewPortFocused = false;
	bool m_IsViewPortHovered = false;

}; 


