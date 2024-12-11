#pragma once
#include "cameraControl.h"

namespace JB
{
	class FPSCameraControl : public CameraControl
	{
	public:
		using Ptr = std::shared_ptr<FPSCameraControl>;

		static FPSCameraControl::Ptr create(std::weak_ptr<Camera> camera);

		FPSCameraControl(std::weak_ptr<Camera> camera);

		~FPSCameraControl();

		void onKeyboard(const KeyBoardState& action) override;

		void onMouseAction(const MouseAction& action) override;

		void onMouseMove(double xpos, double ypos) override;

		void setSpeed(float speed);

		void setSensitivity(bool sensitivity);

		void update();

		void mouseStateReset();

	private:
		float m_speed;					//移动速度
		float m_sensitivity;			//鼠标灵敏度
		bool m_w;
		bool m_s;
		bool m_a;
		bool m_d;
		glm::vec2 m_mousePosition;
		float m_pitchAngle;
		float m_yawAngle;
		glm::vec3 m_front;
		bool m_mouseStateReset;			//重置鼠标状态
		bool m_rotationState;			//旋转状态
	};
}
