#pragma  once

#include "System.h"
#include "Rubber/Renderer/FrameBuffer.h"

namespace Rubber{

	class RendererSystem : public SystemBase{

	public:
		RendererSystem() = default;
		
		void onUpdate(const float ts) override;
		void init(Ref<Scene> scene) override;


	private:
		Ref<FrameBuffer> m_WorldFBO;
		
	};



}