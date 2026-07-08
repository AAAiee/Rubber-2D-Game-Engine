#pragma once
#include "RB/Renderer/Camera.h"

namespace RB{
	class  SceneCamera:  public Camera{

		//fwd

	public:
		enum class ProjectionType { Ortho, Persp };
		SceneCamera();
	    void setProjectionType(ProjectionType type);
		ProjectionType getProjectionType() const;
		void setOrtho(float size,  float aspectRatio, float clipNear, float clipFar);
		void onResize(float width, float height);

	public: //ortho
		inline float getOrthoSize() const {
			return m_OrthoSize;
		}
		inline float getOrthoNearClip() const {
			return m_OrthoNear;
		}
		inline 	float getOrthoFarClip() const {
			return m_OrthoFar;
		}
		inline 	void setOrthoSize(float orthoSize) {
			m_OrthoSize = orthoSize;
			recalculateMatrix();
		}
		inline 	void setOrthoNearClip(float orthoNearClip) {
			m_OrthoNear = orthoNearClip;
			recalculateMatrix();
		}
		inline 	void setOrthoFarClip(float orthoFarClip) {
			m_OrthoFar = orthoFarClip;
			recalculateMatrix();
		}
		
	public://perspective
		inline 	float getPersFoVY() const {
			return m_PersFovY;
		}
		inline 	float getPersNearClip() const {
			return m_PersNear;
		}
		inline 	float getPersFarClip() const {
			return m_PersFar;
		}
		inline 	void setPersFovY(float persFovY) {
			m_PersFovY =  persFovY;
			recalculateMatrix();
		}
		inline 	void setPersNearClip(float persNearClip) {
			m_PersNear = persNearClip;
			recalculateMatrix();
		}
		inline 	void setPersFarClip(float persFarClip) {
			m_PersFar = persFarClip;
			recalculateMatrix();
		}
	private:
		inline 	void recalculateMatrix();
	
	private:
		ProjectionType m_ActiveProjectionType = ProjectionType::Ortho; 

		float m_OrthoSize = 10.0f,  m_OrthoNear = -1.0f, m_OrthoFar = 1.0f; // size = the height of the projection
		float m_PersFovY = glm::radians(45.0f) , m_PersNear = 0.01f, m_PersFar = 10000.f; 

		float m_AspectRatio = 16.0f/ 9.0f;
	};




}

