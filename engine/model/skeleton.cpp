#include "skeleton.h"
#include <iostream>

namespace JB
{
	Skeleton::Skeleton(const std::vector<Bone::Ptr>& bones, const std::vector<glm::mat4>& offsetMatrices) noexcept
	{
		m_bones = bones;
		m_offsetMatrices = offsetMatrices;
	}

	Skeleton::~Skeleton() noexcept
	{
	}

	Bone::Ptr Skeleton::getBone(const std::string& name) noexcept
	{
		for (uint32_t i = 0; i < m_bones.size(); ++i) 
		{
			if (m_bones[i]->getName() == name) 
			{
				return m_bones[i];
			}
		}

		return nullptr;
	}

	void Skeleton::update()
	{
		m_boneMatrices.clear();

		for (uint32_t i = 0; i < m_bones.size(); ++i) 
		{
			auto matrix = m_bones[i]->getModelMatrix() * m_offsetMatrices[i];
			m_boneMatrices.push_back(matrix);
		}
	}

	std::vector<glm::mat4> Skeleton::getBoneMatrices()
	{
		return m_boneMatrices;
	}
}
