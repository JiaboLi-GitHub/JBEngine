#pragma once

#include "../objects/object3D.h"

namespace JB
{
	class Bone : public Object3D
	{
	public:
		using Ptr = std::shared_ptr<Bone>;

		static Ptr create();

		~Bone();

		void updateModelMatrix(bool updateParent = false, bool updateChildren = false) override; 

		void setNodeMatrix(const glm::mat4 nodeMatrix);

		glm::mat4 getNodeMatrix();

		void setName(const std::string& name);

		std::string getName();

	private:
		Bone();

	private:
		std::string m_name;
		glm::mat4 m_nodeMatrix;
	};
}
