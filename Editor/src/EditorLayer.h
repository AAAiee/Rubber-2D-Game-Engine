#pragma once

#include"Panels/SceneHierachyPanel.h"
#include "Utilities/FontsManager/FontManager.h"
#include "Rubber/Scene/Utili/Serializer/SceneSerializer.h"
#include "Rubber/Scene/Utili/Platform/PlatformUtil.h"

#include<Rubber.h>

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

			auto io = ImGui::GetIO();

			FontInfo info;
			info.name = "OpenSans-Bold";
			info.path = "assets/fonts/OpenSans/static/OpenSans-Bold.ttf"	;
			info.size = 18.0f;

			// load fonts
			FontManager::loadFontFromTTF(info);
			info.name = "OpenSans-Regular";
			info.path = "assets/fonts/OpenSans/static/OpenSans-Regular.ttf";
			FontManager::loadFontFromTTF(info);

			// set default font to OpenSans 
			FontManager::setDefaultFont("OpenSans-Regular");

			uint32_t width = Application::getWindow().getWidth();
			uint32_t height = Application::getWindow().getHeight();

			m_FrameBuffer = FrameBuffer::create({ width,height,1,false });
			m_ActiveScene = Scene::create();

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
				m_ActiveScene->updatesViewportSize(m_ViewPortDimension);
			}

			// Update all systems and render everything into the frame buffer
			m_FrameBuffer->bind();
			Renderer2D::resetRendererStat();
			m_ActiveScene->onSceneUpdate(ts);
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

			// config the minimum window width for any docked windows
			float windowMinimumSizeX = ImGui::GetWindowWidth();
			ImGuiStyle& style = ImGui::GetStyle();
			style.WindowMinSize.x = 420.0f;
			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}
			// the rest of windows' minimum width stay what it was
			style.WindowMinSize.x = windowMinimumSizeX;

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File")) {
					if (ImGui::MenuItem("New", "Ctrl+N")) {
						m_ActiveScene = Scene::create();
						m_SceneHierachyPanel.setContext(m_ActiveScene);
					}

                    if (ImGui::MenuItem("Open...", "Ctrl+O")) {
                        std::wstring filepath = fileDialog::openFile(L"Rubber Scene (*.rubber)\0*.rubber\0\0");
                        if (!filepath.empty()) {
                            m_ActiveScene = Scene::create();
                            m_SceneHierachyPanel.setContext(m_ActiveScene);
							m_ActiveScene->updatesViewportSize(m_ViewPortDimension);
							m_ActiveScene->publishViewportResize();
                            SceneSerializer serializer(m_ActiveScene);
                            serializer.deserialize(filepath);
                        }
                    }

                    if (ImGui::MenuItem("Save As...")) {
                        std::wstring filepath = fileDialog::saveFile(L"Rubber Scene (*.rubber)\0*.rubber\0\0");
                        if (!filepath.empty()) {
                            SceneSerializer serializer(m_ActiveScene);
                            serializer.serialize(filepath);
                        }
                    }

					if(ImGui::MenuItem("Exit")) {
						Application::close();
					}

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
			ImGui::Image(reinterpret_cast<void*>((uint64_t)m_FrameBuffer->getColorAttachmentID()), *(ImVec2*)&m_ViewPortDimension, { 0,1 }, { 1,0 });
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


