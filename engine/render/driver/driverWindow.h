#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <functional>
#include "../../global/constant.h"

namespace JB
{
	class Renderer;
	class DriverWindow
	{
	public:
		using Ptr = std::shared_ptr<DriverWindow>;

		//窗口大小变化回调
		using FrameSizeCallback = std::function<void(int width, int height)>;

		//鼠标移动回调
		using MouseMoveCallback = std::function<void(double xpos, double ypos)>;

		//键盘响应回调 
		using KeyboardActionCallback = std::function<void(KeyBoardState)>;

		//鼠标事件响应
		using MouseActionCallback = std::function<void(MouseAction)>;

		DriverWindow(std::weak_ptr<Renderer> renderer, int width, int height);

		~DriverWindow();

		void swap();

		bool processEvent();

		void setFrameSizeCallback(const FrameSizeCallback& callback);

		void setMouseMoveCallBack(const MouseMoveCallback& callback);

		void setMouseActionCallback(const MouseActionCallback& callback);

		void setKeyboardActionCallBack(const KeyboardActionCallback& callback);

	private:
		static void frameSizeCallback(GLFWwindow* window, int width, int height);

		static void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos);

		static void mouseActionCallback(GLFWwindow* window, int button, int action, int mods);

	private:
		std::weak_ptr<Renderer> m_renderer;
		GLFWwindow* m_window = nullptr;
		int m_width;
		int m_height;
		FrameSizeCallback		m_frameSizeCallback;
		MouseMoveCallback		m_mouseMoveCallback;
		KeyboardActionCallback	m_keyboardActionCallback;
		MouseActionCallback		m_mouseActionCallback;
		KeyBoardState m_currentKeyBoardState;	//当前键盘状态
	};
}
