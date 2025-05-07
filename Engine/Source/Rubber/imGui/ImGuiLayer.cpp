#include "pch.h"
#include "ImGuiLayer.h"
#include "imgui.h"

#include <GLFW/glfw3.h>
#include "Platform/OpenGL/imgui_impl_opengl3.h"
#include "Platform/OpenGL/imgui_impl_glfw.h"

#include "Rubber/Application.h"
#include "Rubber/Window.h"
#include "Platform/Windowswindow.h"


Rubber::ImGuiLayer::ImGuiLayer()
{
	// get the window pointer from the application
    auto win = dynamic_cast<WindowsWindow*>(&Application::getInstance().getWindow());
	// check if the window can be casted to WindowsWindow, if not 
	// throw an assertion error
	RB_ASSERT(win, "Application window is not a WindowsWindow");

	// get the window pointer from the WindowsWindow
	m_glfwWindow = win->getWindow();
}

Rubber::ImGuiLayer::~ImGuiLayer()
{
}

void Rubber::ImGuiLayer::onAttach()
{
	// create a new ImGui context
	ImGui::CreateContext();
	// set the ImGui style
	ImGui::StyleColorsDark();
	ImGuiIO& io = ImGui::GetIO();

	// enable keyboard nav
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	// Init the ImGui GLFW and OpenGL3 bindings
	ImGui_ImplGlfw_InitForOpenGL(m_glfwWindow, true);
	ImGui_ImplOpenGL3_Init("#version 410");
}

void Rubber::ImGuiLayer::onDetach()
{
}

void Rubber::ImGuiLayer::onUpdate()
{
	// start a new ImGui frame
	ImGuiIO& io = ImGui::GetIO();

	// get the current time and set the delta time
	float time = (float)glfwGetTime();
	io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
	m_Time = time;

	// begin a new ImGui frame
	ImGui_ImplGlfw_NewFrame();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	// show the ImGui demo window
	static bool showDemo = true;
	ImGui::ShowDemoWindow(&showDemo); 

	// show the ImGui about window
	ImGui::Render(); 
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


