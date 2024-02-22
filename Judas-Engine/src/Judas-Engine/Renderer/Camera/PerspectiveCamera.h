#pragma once

#include <glm/glm.hpp>

namespace Judas_Engine
{
	class PerspectiveCamera
	{
	public:
		PerspectiveCamera(float fov, float aspect, float camNear, float camFar);
		void Reset();

		void SetProjection(float fov, float aspect, float camNear, float camFar);

		const glm::vec3& GetPosition() const { return m_Position; }
		const glm::vec3& GetLocalTargetPosition() const { return m_LocalTargetPosition; }

		void SetPosition(const glm::vec3& position, bool moveTarget = true) { m_Position = position; if (moveTarget) { UpdateTargetPosition(); } RecalculateLocalBasis(); RecalculateViewMatrix(); }
		void SetLocalTargetPosition(const glm::vec3& target) { m_LocalTargetPosition = target; UpdateTargetPosition(); }

		void Translate(const glm::vec3& translation, bool moveTarget = true) { SetPosition(m_Position + translation, moveTarget); }
		void TranslateLocal(const glm::vec3& translation, bool moveTarget = true) { Translate(LocalToWorldCoords(translation), moveTarget); }
		void TranslateTargetLocal(const glm::vec3& translation) { m_LocalTargetPosition += translation; }
		void RotateAroundAxis(const glm::vec3& axis, float rotation);

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:
		void RecalculateViewMatrix();
		void RecalculateLocalBasis();
		void UpdateTargetPosition() { m_Target = m_Position + LocalToWorldCoords(m_LocalTargetPosition); }
		void LookAtTarget();

		const glm::vec3 LocalToWorldCoords(const glm::vec3& a) { return -a.x * m_Right + a.y * m_Up + a.z * m_Direction; }

	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position;
		glm::vec3 m_LocalTargetPosition;
		glm::vec3 m_Target;

		glm::vec3 m_Direction;
		glm::vec3 m_Right;
		glm::vec3 m_Up;
	};
}
