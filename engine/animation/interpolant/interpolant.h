#pragma once

#include <memory>
#include <string>
#include <vector>

namespace JB
{
	// 插值
	class Interpolant
	{
	public:
		using Ptr = std::shared_ptr<Interpolant>;

		Interpolant(const std::vector<float>& times, const std::vector<float>& values, const uint32_t& size, void* result = nullptr);

		~Interpolant();

		void evaluate(float t);

		void setBuffer(float* buffer);

	protected:
		void copySampleValue(const uint32_t& index);

		virtual void interpolateInternal(const uint32_t& lastIndex, const float& lastTime, const float& nextTime, const float& t) = 0;

	protected:
		const std::vector<float>& m_times;
		const std::vector<float>& m_values;
		uint32_t m_size = 0;
		float* m_result = nullptr;
		uint32_t m_cachedIndex = 0;
	};
}

