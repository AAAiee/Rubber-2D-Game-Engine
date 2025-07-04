#pragma once
#include "Rubber/Renderer/Camera.h"

namespace Rubber{
	class  SceneCamera:  public Camera{
		//fwd
     	enum class ProjectionType;

	public:
		SceneCamera();

		void setProjectionType(ProjectionType type){
			m_ActiveProjectionType = type;
		}

		ProjectionType getProjectionType() const {
			return m_ActiveProjectionType;
		}

		void setOrtho(const float size, const float aspectRatio, const float clipNear, const float clipFar) {
			m_OrthoSize = size;
			m_AspectRatio = aspectRatio;
			m_OrthoNear = clipNear;
			m_OrthoFar = clipFar;
			recalculateMatrix();
		}

		void onResize(const float width, const float height);
		
	private:
		void recalculateMatrix();

	private:
		enum class ProjectionType {Ortho, Persp};
		
	private:
		ProjectionType m_ActiveProjectionType = ProjectionType::Ortho; 
		//TODO: Supports perspective projection as well
		float m_OrthoSize = 10.0f,  m_OrthoNear = -1.0f, m_OrthoFar = 1.0f; // size = the height of the projection
		float m_AspectRatio = 1.0f;
	};




}

