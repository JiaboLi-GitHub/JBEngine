#include "linearInterpolant.h"

namespace JB
{
	LinearInterpolant::LinearInterpolant(const std::vector<float>& times, const std::vector<float>& values, const uint32_t& size, void* result)
		: Interpolant(times, values, size, result)
	{

	}

	LinearInterpolant::~LinearInterpolant()
	{
	}

	void LinearInterpolant::interpolateInternal(const uint32_t& lastIndex, const float& lastTime, const float& nextTime, const float& t)
	{
		auto offsetLast = lastIndex * m_size;
		auto offsetNext = offsetLast + m_size;

		auto weightNext = (t - lastTime) / (nextTime - lastTime);
		auto weightLast = 1.0f - weightNext;

		//假设是位置信息，则对xyz分别做了加权相加
		for (uint32_t i = 0; i < m_size; ++i) {
			m_result[i] = m_values[offsetLast + i] * weightLast + m_values[offsetNext + i] * weightNext;
		}
	}
}
