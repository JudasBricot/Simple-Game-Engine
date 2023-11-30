#include "jepch.h"
#include "PerspectiveCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Judas_Engine
{
	PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float camNear, float camFar)
		: m_ProjectionMatrix(glm::perspective(fov, aspect, camNear, camFar)), m_ViewMatrix(glm::mat4(1.0f))
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::SetProjection(float fov, float aspect, float camNear, float camFar)
	{
		m_ProjectionMatrix = glm::perspective(fov, aspect, camNear, camFar);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::RecalculateViewMatrix()
	{
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Direction, m_Up);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::RecalculateLocalBasis()
	{
		m_Direction = glm::normalize(m_Target - m_Position);
		m_Right = glm::normalize(glm::cross(m_Direction, { 0.0f, 1.0f, 0.0f }));
		m_Up = glm::normalize(glm::cross(m_Right, m_Direction));
	}
}
