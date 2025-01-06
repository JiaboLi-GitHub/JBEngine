#include "bone.h"

#include <iostream>

namespace JB
{
	Bone::Ptr Bone::create()
	{
		return std::shared_ptr<Bone>(new Bone());
	}

	Bone::~Bone()
	{
	}

	void Bone::updateModelMatrix(bool updateParent, bool updateChildren)
	{
		auto parent = m_parent.lock();
		if (parent && updateParent)
			parent->updateModelMatrix(true, false);

		updateMatrix();

		m_modelMatrix = m_localMatrix;

		if (parent)
		{
			if (parent->getType() == Object3DType::Bone)
			{

				m_modelMatrix = parent->getModelMatrix() * m_modelMatrix;
			}
		}

		//ûжNodematrix
		if (m_localMatrix == glm::mat4(1.0))
		{
			m_modelMatrix = m_modelMatrix * m_nodeMatrix;
		}

		if (updateChildren) 
		{
			for (auto& child : getChildren()) 
			{
				child->updateModelMatrix(false, true);
			}
		}
	}

	void Bone::setNodeMatrix(const glm::mat4 nodeMatrix)
	{
		m_nodeMatrix = nodeMatrix;
	}

	glm::mat4 Bone::getNodeMatrix()
	{
		return m_nodeMatrix;
	}

	void Bone::setName(const std::string& name)
	{
		m_name = name;
	}

	std::string Bone::getName()
	{
		return m_name;
	}

	Bone::Bone()
	{
		m_type = Object3DType::Bone;
		m_nodeMatrix = glm::mat4(1.0f);
	}
}
