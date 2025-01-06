#include "cameraControl.h"

namespace JB
{
	CameraControl::CameraControl(std::weak_ptr<Camera> camera)
	{
		m_camera = camera;
	}

	CameraControl::~CameraControl()
	{
	}
}
