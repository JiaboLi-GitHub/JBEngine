#include "driverInfo.h"

#include <iostream>

namespace JB
{
	DriverInfo::DriverInfo()
	{
		m_startTime = std::chrono::high_resolution_clock::now();
	}

	DriverInfo::~DriverInfo()
	{
	}
	
	void DriverInfo::addFrameCount()
	{
		++m_frameCount;

		update();
	}
	
	uint32_t DriverInfo::getFPS()
	{
		return m_fps;
	}

	void DriverInfo::update()
	{
		auto currentTime1 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> elapsedTime = currentTime1 - m_startTime;

		if (elapsedTime.count() >= 1.0f)
		{
			m_fps = m_frameCount / elapsedTime.count();
			m_frameCount = 0;
			m_startTime = currentTime1;

			std::cout << m_fps << std::endl;
		}
	}
}
