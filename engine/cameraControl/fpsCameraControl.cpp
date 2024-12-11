#include "fpsCameraControl.h"

namespace JB
{
	FPSCameraControl::Ptr FPSCameraControl::create(std::weak_ptr<Camera> camera)
	{
		return std::make_shared<FPSCameraControl>(camera);
	}

	FPSCameraControl::FPSCameraControl(std::weak_ptr<Camera> camera)
		: CameraControl(camera)
	{
		m_speed = 0.1f;
		m_sensitivity = 0.1f;
		m_w = m_s = m_a = m_d = false;
		m_pitchAngle = 0.0f;
		m_yawAngle = -90.0f;
		m_front = glm::vec3(0.0f);
		m_mouseStateReset = true;
		m_mousePosition = glm::vec2(0.0f);
		m_rotationState = false;
	}

	FPSCameraControl::~FPSCameraControl()
	{
	}

	void FPSCameraControl::onKeyboard(const KeyBoardState& action)
	{
		m_w = action[static_cast<uint32_t>(KeyAction::Key_W)];
		m_s = action[static_cast<uint32_t>(KeyAction::Key_S)];
		m_a = action[static_cast<uint32_t>(KeyAction::Key_A)];
		m_d = action[static_cast<uint32_t>(KeyAction::Key_D)];
	}

	void FPSCameraControl::onMouseAction(const MouseAction& action)
	{
		if (action == MouseAction::RightDown) {
			m_rotationState = true;
		}
		else if (action == MouseAction::RightUp) {
			m_rotationState = false;
			//用来标定，是否第一次按下鼠标
			m_mouseStateReset = true;
		}
	}

	void FPSCameraControl::onMouseMove(double xpos, double ypos)
	{
		if (!m_rotationState)
			return;

		if (m_mouseStateReset)
		{
			m_mouseStateReset = false;
		}
		else
		{

			float deltaX = static_cast<float>(xpos - m_mousePosition.x);
			float deltaY = static_cast<float>(m_mousePosition.y - ypos);

			float angleX = deltaX * m_sensitivity;
			float angleY = deltaY * m_sensitivity;

			m_pitchAngle += angleY;
			m_yawAngle += angleX;

			//不能仰面翻过去，也不能前滚翻
			if (m_pitchAngle >= 90.0f) {
				m_pitchAngle = 89.0f;
			}

			if (m_pitchAngle <= -90.0f) {
				m_pitchAngle = -89.0f;
			}

			m_front.y = sin(glm::radians(m_pitchAngle));
			m_front.x = cos(glm::radians(m_yawAngle)) * cos(glm::radians(m_pitchAngle));
			m_front.z = sin(glm::radians(m_yawAngle)) * cos(glm::radians(m_pitchAngle));

			//一定要注意传进去的是看向哪个点，而不是直接把方向传进去
			auto camera = m_camera.lock();
			camera->lookAt(m_front + camera->getPosition(), glm::vec3(0.0, 1.0, 0.0));
		}

		m_mousePosition.x = static_cast<float>(xpos);
		m_mousePosition.y = static_cast<float>(ypos);
	}

	void FPSCameraControl::setSpeed(float speed)
	{
		m_speed = speed;
	}

	void FPSCameraControl::setSensitivity(bool sensitivity)
	{
		m_sensitivity = sensitivity;
	}

	void FPSCameraControl::update()
	{
		auto camera = m_camera.lock();

		glm::vec3 front = camera->getFront();

		glm::vec3 right = camera->getRight();

		glm::vec3 position = camera->getPosition();

		glm::vec3 direction = glm::vec3(0.0f);
		bool needsUpdate = false;

		if (m_w)
		{
			direction += front;
			needsUpdate = true;
		}

		if (m_s)
		{
			direction += -front;
			needsUpdate = true;
		}

		if (m_a)
		{
			direction += -right;
			needsUpdate = true;
		}

		if (m_d)
		{
			direction += right;
			needsUpdate = true;
		}

		if (needsUpdate && (direction.x != 0.0 || direction.y != 0.0 || direction.z != 0.0))
		{
			direction = glm::normalize(direction);
			position += direction * m_speed;

			camera->setPosition(position);
		}
	}

	void FPSCameraControl::mouseStateReset()
	{
		m_mouseStateReset = true;
	}
}
