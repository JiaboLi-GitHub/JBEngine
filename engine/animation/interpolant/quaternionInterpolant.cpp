#include "quaternionInterpolant.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace JB
{
	QuaternionInterpolant::QuaternionInterpolant(const std::vector<float>& times, const std::vector<float>& values, const uint32_t& size, void* result)
		: Interpolant(times, values, size, result)
	{
	}

	QuaternionInterpolant::~QuaternionInterpolant()
	{
	}

	void QuaternionInterpolant::interpolateInternal(const uint32_t& lastIndex, const float& lastTime, const float& nextTime, const float& t)
	{
		auto offsetLast = lastIndex * m_size;
		auto offsetNext = offsetLast + m_size;

		auto weight = (t - lastTime) / (nextTime - lastTime);

		glm::quat lastQuat = glm::quat(
			m_values[offsetLast + 3],
			m_values[offsetLast + 0],
			m_values[offsetLast + 1],
			m_values[offsetLast + 2]
		);

		glm::quat nextQuat = glm::quat(
			m_values[offsetNext + 3],
			m_values[offsetNext + 0],
			m_values[offsetNext + 1],
			m_values[offsetNext + 2]
		);

		glm::quat resultQuat = glm::slerp(lastQuat, nextQuat, weight);

		m_result[0] = resultQuat.x;
		m_result[1] = resultQuat.y;
		m_result[2] = resultQuat.z;
		m_result[3] = resultQuat.w;
	}
}
