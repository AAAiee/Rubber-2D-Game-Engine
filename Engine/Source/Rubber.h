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
#include "Rubber/Utility/Timer/GameLoopTimer.h"

//Utilities
#include "Rubber/Utility/Utility.h"
#include "Rubber/Core/Search.h"

//ECS
#include "Rubber/Scene/Scene.h"
#include "Rubber/Scene/Utili/Component.h"
#include "Rubber/Scene/Utili/Entity.h"
#include "Rubber/Scene/System/System.h"
#include "Rubber/Scene/System/AnimationSystem.h"
#include "Rubber/Scene/System/ScriptSystem.h"
#include "Rubber/Scene/System/CameraSystem.h"
#include "Rubber/Scene/System/FsmSystem.h"
#include "Rubber/Scene/System/RenderSystem.h"
#include "Rubber/Scene/System/InputSystem.h"
#include "Rubber/Scene/System/WindowSystem.h"
#include "Rubber/Scene/System/MoveSystem.h"
#include "Rubber/Scene/Utili/ScriptableEntity.h"
#include "Rubber/Scene/System/CollisionSystem.h"
#include "Rubber/Scene/System/FsmPostMoving.h"


//Asset Manager
#include "Rubber/Resources/AssetManager.h"
#include "Rubber/Resources/AssetMetaData.h"
#include "Rubber/Resources/Asset.h"
#include "Rubber/Resources/AnimationResources/AnimationAtlas.h"
#include "Rubber/Resources/AnimationResources/AnimationSpriteSheet.h"


namespace RB = Rubber;


