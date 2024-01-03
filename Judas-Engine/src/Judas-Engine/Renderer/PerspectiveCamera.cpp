#include "jepch.h"
#include "PerspectiveCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Judas_Engine
{
	PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float camNear, float camFar)
		: m_ProjectionMatrix(glm::perspective(fov, aspect, camNear, camFar)), m_ViewMatrix(glm::mat4(1.0f))
	{
		Reset();
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::Reset()
	{
		m_Position = { 0.0f, 0.0f, 3.0f };
		m_LocalTargetPosition = { 0.0f, 0.0f, 3.0f };
		m_Target = { 0.0f, 0.0f, 0.0f };

		m_Direction = glm::vec3(0, 0, -1);
		m_Right = glm::vec3(-1, 0, 0);
		m_Up = glm::vec3(0, 1, 0);

		RecalculateViewMatrix();
	}

	void PerspectiveCamera::SetProjection(float fov, float aspect, float camNear, float camFar)
	{
		m_ProjectionMatrix = glm::perspective(fov, aspect, camNear, camFar);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::RotateAroundAxis(const glm::vec3& axis, float angle)
	{
		glm::vec3 centeredPosition = glm::vec3(m_Position - m_Target);
		glm::mat4x4 rotationMatrix = glm::rotate(glm::mat4x4(1.0f), angle, LocalToWorldCoords(axis));

		glm::vec3 rotatedPosition = rotationMatrix * glm::vec4(centeredPosition, 1.0f);
		Translate(rotatedPosition - centeredPosition, false);

		LookAtTarget();
	}

	void PerspectiveCamera::RecalculateViewMatrix()
	{
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Direction, m_Up);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::RecalculateLocalBasis()
	{
		m_Right = glm::normalize(glm::cross(m_Direction, { 0.0f, 1.0f, 0.0f }));
		m_Up = glm::normalize(glm::cross(m_Right, m_Direction));
	}

	void PerspectiveCamera::LookAtTarget()
	{
		m_Direction = glm::normalize(m_Target - m_Position);
		RecalculateLocalBasis();
	}
}
