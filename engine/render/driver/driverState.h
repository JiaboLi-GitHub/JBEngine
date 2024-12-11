#pragma once
#include <memory>
#include "../../global/constant.h"

namespace JB
{
	class DriverState
	{
	public:
		using Ptr = std::shared_ptr<DriverState>;

		DriverState();

		~DriverState();

		void setClearColor(float r, float g, float b, float a);

		void setDepthTest(bool state);

		bool getDepthTest();

		void clearBufferBit();

		void setDepthFunc(GLint depthFunc);

		void setViewport(int x, int y, int width, int height);

		void use();

	private:
		bool m_depthTest = true;
		GLint m_depthFunc = GL_LESS;
		glm::vec4 m_clearColor;
		int m_x;
		int m_y;
		int m_width;
		int m_height;
	};
}
