#pragma once

#include <memory>
#include <chrono>

namespace JB
{
	class DriverInfo
	{
	public:
		using Ptr = std::shared_ptr<DriverInfo>;

		DriverInfo();

		~DriverInfo();

		void addFrameCount();

		uint32_t getFPS();

	private:
		void update();

	private:
		uint32_t m_frameCount = 0;
		uint32_t m_fps = 0;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
	};
}

