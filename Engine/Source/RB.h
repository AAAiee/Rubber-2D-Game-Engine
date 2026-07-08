#pragma once
// this is for client-use only
// include the application base class
#include "RB/Core/Application.h"

// include the Layer System
#include "RB/Layer/Layer.h"

// include the logger
#include "RB/Log/Log.h"

// include the ImGui Layer 
#include "RB/imGui/ImGuiLayer.h"

// include the event system
#include "RB/Event/Event.h"
#include "RB/Event/AppEvent.h"
#include "RB/Event/EventManager.h"

// include the Input System and KeyCode
#include "RB/Input/Input.h"
#include "RB/Input/KeyCodes.h"
#include "RB/Input/MouseCodes.h"

#include "RB/Window/Window.h"

//include renderer
#include "RB/Renderer/Renderer.h"
#include "RB/Renderer/Renderer2D.h"
#include "RB/Renderer/BufferLayout.h"
#include "RB/Renderer/Buffers.h"
#include "RB/Renderer/RendererCommand.h"
#include "RB/Renderer/Shaders.h"
#include "RB/Renderer/VertexArray.h"
#include "RB/Renderer/CameraController.h"
#include "RB/Renderer/Texture.h"
#include "RB/Renderer/Subtexture2D.h"
#include "RB/Renderer/FrameBuffer.h"
#include "RB/Renderer/OrthoCameraController.h"

// profile
#include "RB/Debug/Profiler.h"
#include "RB/Utility/Timer/GameLoopTimer.h"

//Utilities
#include "RB/Utility/Utility.h"
#include "RB/Core/Search.h"

//ECS
#include "RB/Scene/Scene.h"
#include "RB/Scene/Utili/Component.h"
#include "RB/Scene/Utili/Entity.h"
#include "RB/Scene/System/System.h"
#include "RB/Scene/System/AnimationSystem.h"
#include "RB/Scene/System/ScriptSystem.h"
#include "RB/Scene/System/CameraSystem.h"
#include "RB/Scene/System/FsmSystem.h"
#include "RB/Scene/System/RenderSystem.h"
#include "RB/Scene/System/InputSystem.h"
#include "RB/Scene/System/WindowSystem.h"
#include "RB/Scene/System/MoveSystem.h"
#include "RB/Scene/Utili/ScriptableEntity.h"
#include "RB/Scene/System/CollisionSystem.h"
#include "RB/Scene/System/FsmPostMoving.h"


//Asset Manager
#include "RB/Resources/AssetManager.h"
#include "RB/Resources/AssetMetaData.h"
#include "RB/Resources/Asset.h"
#include "RB/Resources/AnimationResources/AnimationAtlas.h"
#include "RB/Resources/AnimationResources/AnimationSpriteSheet.h"

