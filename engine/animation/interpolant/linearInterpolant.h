#pragma once

#include "interpolant.h"

namespace JB
{
	// 线性插值
	class LinearInterpolant : public Interpolant
	{
	public:
		LinearInterpolant(const std::vector<float>& times, const std::vector<float>& values, const uint32_t& size, void* result = nullptr);

		~LinearInterpolant();

	private:
		void interpolateInternal(const uint32_t& lastIndex, const float& lastTime, const float& nextTime, const float& t) override;
	};
}
