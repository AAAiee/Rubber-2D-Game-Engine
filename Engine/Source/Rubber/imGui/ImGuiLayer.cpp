#include "pch.h"
#include "ImGuiLayer.h"
#include "imgui.h"

#include <GLFW/glfw3.h>
#include "Platform/OpenGL/imgui_impl_opengl3.h"
#include "Platform/OpenGL/imgui_impl_glfw.h"

#include "Rubber/Application.h"
#include "Rubber/Window.h"
#include "Platform/Windows/Windowswindow.h"


Rubber::ImGuiLayer::ImGuiLayer()
	:m_Time(0.0f)
{

}

Rubber::ImGuiLayer::~ImGuiLayer()
{
	onDetach();
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
	ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(Application::getWindow().getNativeWindow()), true);
	ImGui_ImplOpenGL3_Init("#version 410");
}

void Rubber::ImGuiLayer::onDetach(){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
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


