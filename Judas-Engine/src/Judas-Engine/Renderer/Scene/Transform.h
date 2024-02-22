#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/quaternion.hpp>

// TEMP
#include <glm/gtx/string_cast.hpp>

namespace Judas_Engine
{
	class Transform
	{
	public:
		Transform() {}
		Transform(const glm::vec3& position);
		Transform(const glm::vec3& position, const glm::vec3& scale);

		const glm::mat4 GetModelMatrix() { 
			return m_TransformMatrix * glm::toMat4(m_Rotation) * m_ScaleMatrix;
		}

		void SetPosition(const glm::vec3& position) { m_Position = position; glm::translate(glm::mat4(1.0), m_Position); }
		const glm::vec3& GetPosition() { return m_Position; }

		void SetRotation(const glm::vec3& axis, float angle) { glm::rotate(glm::mat4(1.0), angle, axis); }
		const glm::quat& GetRotation() { return m_Rotation; }

		void SetScale(const glm::vec3 scale) { m_Scale = scale; glm::scale(glm::mat4(1.0), m_Scale); }
		const glm::vec3& GetScale() { return m_Scale; }

	private:
		glm::vec3 m_Position = glm::vec3();
		glm::quat m_Rotation = glm::quat();
		glm::vec3 m_Scale = glm::vec3();

		glm::mat4 m_TransformMatrix = glm::mat4(1.0);
		//glm::mat4 m_RotationMatrix = glm::mat4();
		glm::mat4 m_ScaleMatrix = glm::mat4(1.0);
	};
}
