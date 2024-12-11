#pragma once
#include "../camera/camera.h"

namespace JB
{
	class CameraControl
	{
	public:
		CameraControl(std::weak_ptr<Camera> camera);

		~CameraControl();

		virtual void onKeyboard(const KeyBoardState& action) = 0;

		virtual void onMouseAction(const MouseAction& action) = 0;

		virtual void onMouseMove(double xpos, double ypos) = 0;

	protected:
		std::weak_ptr<Camera> m_camera;
	};
}
