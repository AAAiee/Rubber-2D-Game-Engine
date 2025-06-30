#pragma once
// this is for client-use only
// include the application base class
#include "Rubber/Core/Application.h"

// include the Layer System
#include "Rubber/Layer/Layer.h"

// include the logger
#include "Rubber/Log/Log.h"

// include the ImGui Layer 
#include "Rubber/imGui/ImGuiLayer.h"

// include the event system
#include "Rubber/Event/Event.h"
#include "Rubber/Event/AppEvent.h"
#include "Rubber/Event/EventManager.h"

// include the Input System and KeyCode
#include "Rubber/Input/Input.h"
#include "Rubber/Input/KeyCodes.h"
#include "Rubber/Input/MouseCodes.h"

#include "Rubber/Window/Window.h"

//include renderer
#include "Rubber/Renderer/Renderer.h"
#include "Rubber/Renderer/Renderer2D.h"
#include "Rubber/Renderer/BufferLayout.h"
#include "Rubber/Renderer/Buffers.h"
#include "Rubber/Renderer/RendererCommand.h"
#include "Rubber/Renderer/Shaders.h"
#include "Rubber/Renderer/VertexArray.h"
#include "Rubber/Renderer/CameraController.h"
#include "Rubber/Renderer/Texture.h"
#include "Rubber/Renderer/Subtexture2D.h"
#include "Rubber/Renderer/FrameBuffer.h"
#include "Rubber/Renderer/OrthoCameraController.h"

// profile
#include "Rubber/Debug/Profiler.h"
#include "Rubber/Timer/Timer.h"


//Utilities
#include "Rubber/Utility/Utility.h"


