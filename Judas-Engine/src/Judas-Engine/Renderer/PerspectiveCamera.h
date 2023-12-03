#pragma once

#include <glm/glm.hpp>

namespace Judas_Engine
{
	class PerspectiveCamera
	{
	public:
		PerspectiveCamera(float fov, float aspect, float camNear, float camFar);

		void SetProjection(float fov, float aspect, float camNear, float camFar);

		const glm::vec3 GetPosition() const { return m_Position; }
		const glm::vec3& GetRotation() const { return m_Rotation; }

		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateLocalBasis(); RecalculateViewMatrix(); }
		void SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; RecalculateLocalBasis(); RecalculateViewMatrix(); }
		void SetTarget(glm::vec3 target) { m_Target = target; RecalculateLocalBasis(); }

		void Translate(const glm::vec3& translation) { SetPosition(m_Position + translation); }
		void TranslateLocal(const glm::vec3& translation) { Translate(-translation.x * m_Right + translation.y * m_Up + translation.z * m_Direction); }
		void Rotate(const glm::vec3& rotation) { SetRotation(m_Rotation + rotation); }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:
		void RecalculateViewMatrix();
		void RecalculateLocalBasis();

	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 3.0f };

		glm::vec3 m_Direction = glm::vec3(0, 0, -1);
		glm::vec3 m_Right = glm::vec3(-1, 0, 0);
		glm::vec3 m_Up = glm::vec3(0, 1, 0);

		glm::vec3 m_Rotation;

		glm::vec3 m_Target = { 0.0f, 0.0f, 0.0f };
	};
}
