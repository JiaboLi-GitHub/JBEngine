#pragma once

#include "interpolant.h"

namespace JB
{
	// 球面插值
	class QuaternionInterpolant : public Interpolant
	{
	public:
		QuaternionInterpolant(const std::vector<float>& times, const std::vector<float>& values, const uint32_t& size, void* result = nullptr);

		~QuaternionInterpolant();

	private:
		void interpolateInternal(const uint32_t& lastIndex, const float& lastTime, const float& nextTime, const float& t) override;
	};
}

