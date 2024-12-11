#pragma once
#include "../objects/object3D.h"

namespace JB
{
	class Camera : public Object3D
	{
	public:
		using Ptr = std::shared_ptr<Camera>;

		~Camera();

		virtual void updateModelMatrix(bool updateParent = false, bool updateChildren = false) override;

		glm::mat4x4 getModelMatrixInverse();

		glm::mat4x4 getProjectionMatrix();

		virtual void updateProjectionMatrix() = 0;

	protected:
		Camera();

	protected:
		glm::mat4x4 m_modelMatrixInverse;
		glm::mat4x4 m_projectionMatrix;
	};
}
