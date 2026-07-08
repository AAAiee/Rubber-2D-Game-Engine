#pragma  once

#include "System.h"
#include "RB/Renderer/FrameBuffer.h"

namespace RB{

	class RendererSystem : public SystemBase{

	public:
		RendererSystem() = default;
		
		void onUpdate(const float ts) override;
		void init(Ref<Scene> scene) override;


	private:
		Ref<FrameBuffer> m_WorldFBO;
		
	};



}