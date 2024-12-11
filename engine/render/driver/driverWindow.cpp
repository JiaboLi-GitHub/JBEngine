#include "driverWindow.h"
#include <iostream>
#include "../render.h"

namespace JB
{
	DriverWindow::DriverWindow(std::weak_ptr<Renderer> renderer, int width, int height)
	{
		m_renderer = renderer;
		m_width = width;
		m_height = height;
		m_frameSizeCallback = nullptr;
		m_mouseMoveCallback = nullptr;
		m_keyboardActionCallback = nullptr;
		m_mouseActionCallback = nullptr;

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

		m_window = glfwCreateWindow(width, height, "JBEngine window", nullptr, nullptr);
		if (!m_window) {
			std::cerr << "Error: failed to create window" << std::endl;
			glfwTerminate();
			exit(0);
		}

		glfwMakeContextCurrent(m_window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			exit(0);
		}

		glfwSetWindowUserPointer(m_window, this);

		glfwSetFramebufferSizeCallback(m_window, frameSizeCallback);

		glfwSetMouseButtonCallback(m_window, mouseActionCallback);

		glfwSetCursorPosCallback(m_window, mouseMoveCallback);

		glfwSwapInterval(1); // 启用 V-Sync

		// 启用多重采样
		glfwWindowHint(GLFW_SAMPLES, 4);
	}

	DriverWindow::~DriverWindow()
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	void DriverWindow::swap()
	{
		glfwSwapBuffers(m_window);
	}

	bool DriverWindow::processEvent()
	{
		if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(m_window))
		{
			glfwSetWindowShouldClose(m_window, true);
			return false;
		}

		glfwPollEvents();

		KeyBoardState keyBoardState;
		for (const auto& [fwKey, keyAction] : KeyActionMap)
		{
			if (glfwGetKey(m_window, fwKey) == GLFW_PRESS)
			{
				keyBoardState[static_cast<uint32_t>(keyAction)] = 1;
			}
			else if (glfwGetKey(m_window, fwKey) == GLFW_RELEASE)
			{
				keyBoardState[static_cast<uint32_t>(keyAction)] = 0;
			}
		}

		if (m_keyboardActionCallback && m_currentKeyBoardState != keyBoardState)
		{
			m_currentKeyBoardState = keyBoardState;
			m_keyboardActionCallback(keyBoardState);
		}

		return true;
	}

	void DriverWindow::setFrameSizeCallback(const FrameSizeCallback& callback)
	{
		m_frameSizeCallback = callback;
	}

	void DriverWindow::setMouseMoveCallBack(const MouseMoveCallback& callback)
	{
		m_mouseMoveCallback = callback;
	}

	void DriverWindow::setMouseActionCallback(const MouseActionCallback& callback)
	{
		m_mouseActionCallback = callback;
	}

	void DriverWindow::setKeyboardActionCallBack(const KeyboardActionCallback& callback)
	{
		m_keyboardActionCallback = callback;
	}

	void DriverWindow::frameSizeCallback(GLFWwindow* window, int width, int height)
	{
		auto driverWindow = static_cast<DriverWindow*>(glfwGetWindowUserPointer(window));
		if (!driverWindow || !width || !height)
			return;

		driverWindow->m_width = width;
		driverWindow->m_height = height;
		if (!driverWindow->m_renderer.expired())
			driverWindow->m_renderer.lock()->setSize(width, height);
		if (driverWindow->m_frameSizeCallback) 
			driverWindow->m_frameSizeCallback(width, height);

	}

	void DriverWindow::mouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
	{
		auto driverWindow = static_cast<DriverWindow*>(glfwGetWindowUserPointer(window));
		if (!driverWindow)
			return;

		if (driverWindow->m_mouseMoveCallback)
			driverWindow->m_mouseMoveCallback(xpos, ypos);
	}

	void DriverWindow::mouseActionCallback(GLFWwindow* window, int button, int action, int mods)
	{
		auto driverWindow = static_cast<DriverWindow*>(glfwGetWindowUserPointer(window));
		if (!driverWindow)
			return;

		MouseAction mouseAction = MouseAction::None;
		if (action == GLFW_PRESS) 
		{
			if (button == GLFW_MOUSE_BUTTON_1)
			{
				mouseAction = MouseAction::LeftDown;
			}
			else if (button == GLFW_MOUSE_BUTTON_2)
			{
				mouseAction = MouseAction::RightDown;
			}
			else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
			{
				mouseAction = MouseAction::MiddleDown;
			}
		}
		else if (action == GLFW_RELEASE) 
		{
			if (button == GLFW_MOUSE_BUTTON_1)
			{
				mouseAction = MouseAction::LeftUp;
			}
			else if (button == GLFW_MOUSE_BUTTON_2)
			{
				mouseAction = MouseAction::RightUp;
			}
			else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
			{
				mouseAction = MouseAction::MiddleUp;
			}
		}

		if (driverWindow->m_mouseActionCallback)
			driverWindow->m_mouseActionCallback(mouseAction);
	}
}
