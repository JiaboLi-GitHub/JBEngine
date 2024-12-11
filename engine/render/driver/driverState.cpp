#include "driverState.h"

namespace JB
{
	DriverState::DriverState()
	{
		setDepthTest(true);

		glDepthFunc(GL_LEQUAL);
	}

	DriverState::~DriverState()
	{
	}

	void DriverState::setClearColor(float r, float g, float b, float a)
	{
		m_clearColor = glm::vec4(r, g, b, a);
		glClearColor(r, g, b, a);
	}

	void DriverState::setDepthTest(bool state)
	{
		m_depthTest = state;

		if (state)
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
	}

	bool DriverState::getDepthTest()
	{
		return m_depthTest;
	}

	void DriverState::clearBufferBit()
	{
		uint32_t bufferBit = GL_COLOR_BUFFER_BIT;
		if (m_depthTest)
			bufferBit |= GL_DEPTH_BUFFER_BIT;

		glClear(bufferBit);
	}

	void DriverState::setDepthFunc(GLint depthFunc)
	{
		m_depthFunc = depthFunc;
		glDepthFunc(depthFunc);
	}

	void DriverState::setViewport(int x, int y, int width, int height)
	{
		m_x = x;
		m_y = y;
		m_width = width;
		m_height = height;

		glViewport(x, y, width, height);
	}

	void DriverState::use()
	{
		setClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
		setDepthTest(m_depthTest);
		setDepthFunc(m_depthFunc);
		setViewport(m_x, m_y, m_width, m_height);
		clearBufferBit();
	}
}
