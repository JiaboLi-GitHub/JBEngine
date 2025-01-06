#include "interpolant.h"

namespace JB
{
	Interpolant::Interpolant(const std::vector<float>& times, const std::vector<float>& values, const uint32_t& size, void* result)
		: m_times(times),
		  m_values(values),
		  m_size(size),
		  m_result(static_cast<float*>(result))
	{
	}

	Interpolant::~Interpolant()
	{
	}

	void Interpolant::evaluate(float t)
	{
		const auto& pp = m_times;

		uint32_t	lastIndex = 0;

		if (m_cachedIndex != 0) 
		{
			lastIndex = m_cachedIndex - 1;
		}

		if (pp.size() == 1) 
		{
			copySampleValue(0);
			return;
		}
		else 
		{
			if (t < pp[0]) 
			{
				copySampleValue(0);
				return;
			}

			if (t > pp[pp.size() - 1]) 
			{
				copySampleValue(pp.size() - 1);
				return;
			}

			if (t < pp[lastIndex]) 
			{
				lastIndex = 0;
			}

			while (t > pp[lastIndex + 1]) 
			{
				lastIndex++;
			}
		}

		m_cachedIndex = lastIndex + 1;
		interpolateInternal(lastIndex, m_times[lastIndex], m_times[lastIndex + 1], t);
	}

	void Interpolant::setBuffer(float* buffer)
	{
		m_result = buffer;
	}

	void Interpolant::copySampleValue(const uint32_t& index)
	{
		if (index > m_times.size() - 1) 
			return;

		for (uint32_t i = 0; i < m_size; ++i) {
			m_result[i] = m_values[index * m_size + i];
		}
	}
}
