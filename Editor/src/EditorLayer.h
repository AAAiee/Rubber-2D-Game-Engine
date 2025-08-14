#pragma once

#include<Rubber.h>

#include"Panels/SceneHierachyPanel.h"
#include <glm/glm.hpp>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>



namespace Rubber {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer()
			:Layer("example")
		{

		}

		void onAttach() override {
			uint32_t width = Application::getWindow().getWidth();
			uint32_t height = Application::getWindow().getHeight();

			m_FrameBuffer = FrameBuffer::create({ width,height,1,false });
			m_ActiveScene = Scene::create();
			
			// systems init
			m_ActiveScene->addSystem(makeScope <WindowSystem>());
			m_ActiveScene->addSystem(makeScope <RendererSystem>());
			m_ActiveScene->systemsInit();

			m_SquareEntity = m_ActiveScene->createEntity("Square Entity"); 
			m_SquareEntity.addComponent<TransformComponent>(m_SquarePosition, glm::vec2{1.0f, 1.0f}, 0.0f);
			m_SquareEntity.addComponent<SpriteComponent>();
			m_SquareEntity.addComponent<VisibilityControlComponent>(true);
			m_SquareEntity.getComponent<SpriteComponent>().color = m_SquareColor;

			m_Camera = m_ActiveScene->createEntity("Main Camera");
			m_CameraPos = { 0.0f, 1.f, 0.0f };
			m_Camera.addComponent<TransformComponent>(m_CameraPos, glm::vec2{1.0f, 1.0f}, 0.0f);
			m_Camera.addComponent<CameraComponent>();

			m_SecondCamera = m_ActiveScene->createEntity("Second Camera");
			m_SecondCamera.addComponent<TransformComponent>(m_SecondCameraPos, glm::vec2{1.0f, 1.0f}, 0.0f);
			m_SecondCamera.addComponent<CameraComponent>();

			m_SceneHierachyPanel.setContext(m_ActiveScene);
		}

		void onDetach() {

		}

		void onUpdate(const float ts) override
		{
			// Resize the frame buffer to match the view port's size 
			const FrameBufferSpecification& frameBufferSpec = m_FrameBuffer->getSpecification();
			glm::vec2 frameBufferSizeBefore = { frameBufferSpec.m_Width, frameBufferSpec.m_Height };

			// If viewport changed to  0 * anything,invalid dimension , otherwise resize framebuffer
			if (m_ViewPortDimension.x * m_ViewPortDimension.y != 0.0f && frameBufferSizeBefore != m_ViewPortDimension) {
				m_FrameBuffer->resize((uint32_t)m_ViewPortDimension.x, (uint32_t)m_ViewPortDimension.y);
				Application::getEventManager()->enqueue(ViewPortResizeEvent((uint32_t)m_ViewPortDimension.x,(uint32_t)m_ViewPortDimension.y));
			}

			// Update all systems and render everything into the frame buffer
			m_FrameBuffer->bind();
			Renderer2D::resetRendererStat();
			m_ActiveScene->onSystemsUpdate(ts);
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

			// Stats Panel 
			ImGui::Begin("Setting");
			ImGui::Text("DrawCount: %d", Renderer2D::getDrawCallCount());
			ImGui::Text("QuadCount: %d", Renderer2D::getQuadCount());
			ImGui::Text("IndexCount: %d", Renderer2D::getIndexCount());
			ImGui::Text("VertexCount: %d", Renderer2D::getVertexCount());
			ImGui::Text("Frame Time: %f", Application::getTimer()->getAverageFrameTime());
			ImGui::Text("Current FPS : %d", Application::getTimer()->getFps());
			ImGui::End();

			m_SceneHierachyPanel.onImGuiRender();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f,0.0f });
			ImGui::Begin("ViewPort");

			//TODO:: Only Enable zoom callback when is focused or hovered
			m_IsViewPortFocused = ImGui::IsWindowFocused();
			m_IsViewPortHovered = ImGui::IsWindowHovered();

			ImVec2 curViewPortSize = ImGui::GetContentRegionAvail();
			m_ViewPortDimension = { curViewPortSize.x, curViewPortSize.y };
			ImGui::Image(reinterpret_cast<void*>(m_FrameBuffer->getColorAttachmentID()), *(ImVec2*)&m_ViewPortDimension, { 0,1 }, { 1,0 });
			ImGui::End();
			ImGui::PopStyleVar();


			ImGui::End();
		}

	private:
		Ref<FrameBuffer> m_FrameBuffer;
		Ref<Scene> m_ActiveScene;
		SceneHierachyPanel m_SceneHierachyPanel;
		Entity m_SquareEntity;
		Entity m_SecondCamera;
		Entity m_Camera;

	private:
		glm::vec2 m_ViewPortDimension{};
		bool m_IsViewPortFocused = false;
		bool m_IsViewPortHovered = false;

	//test, to be removed
	private:
		glm::vec3 m_SquarePosition = glm::vec3(0.0f);
		glm::vec4 m_SquareColor = glm::vec4(1.0f);
		glm::vec3 m_CameraPos = glm::vec3(0.0f);
		glm::vec3 m_SecondCameraPos = glm::vec3(0.0f);
	};
}


