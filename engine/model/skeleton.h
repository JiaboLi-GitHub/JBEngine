#pragma once

#include "bone.h"

namespace JB
{
	class Skeleton
	{
	public:
		using Ptr = std::shared_ptr<Skeleton>;

		Skeleton(const std::vector<Bone::Ptr>& bones, const std::vector<glm::mat4>& offsetMatrices) noexcept;

		~Skeleton() noexcept;

		Bone::Ptr getBone(const std::string& name) noexcept;

		void update();

		std::vector<glm::mat4> getBoneMatrices();

	private:
		std::vector<Bone::Ptr> m_bones;
		std::vector<glm::mat4> m_offsetMatrices;
		std::vector<glm::mat4> m_boneMatrices;
	};
}
