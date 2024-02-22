#include "jepch.h"
#include "Transform.h"

namespace Judas_Engine
{
	Transform::Transform(const glm::vec3& position)
	{
		SetPosition(position);
	}

	Transform::Transform(const glm::vec3& position, const glm::vec3& scale)
	{
		SetPosition(position);
		SetScale(scale);
	}
}
