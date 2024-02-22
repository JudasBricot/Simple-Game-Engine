#pragma once

#include "Judas-Engine/Renderer/Camera/PerspectiveCamera.h"
#include "Judas-Engine/Core/Timestep.h"

#include "Judas-Engine/EventSystem/ApplicationEvent.h"

namespace Judas_Engine
{
	class PerspectiveCameraController
	{
	public:
		PerspectiveCameraController(float fov, float aspectRatio, float camNear, float camFar);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		const PerspectiveCamera& GetCamera() const { return m_Camera; }
		PerspectiveCamera& GetCamera() { return m_Camera; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnMouseDragged(MouseDraggedEvent& e);
		bool OnWindowResized(WindowResizedEvent& e);
	private:
		float m_Fov;
		float m_AspectRatio;
		float m_Near;
		float m_Far;

		PerspectiveCamera m_Camera;

		float m_CameraTranslationSpeed = 3.0f, m_CameraRotationSpeed = 1.0f, m_DraggingSpeed = 0.01f;

		float m_SlowingZoomZoneRadius = 5.0f, m_SlowingFactor = 10.0f, m_MaxZoomLevel = 1.0f;
	};
}

