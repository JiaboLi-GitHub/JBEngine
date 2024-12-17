#pragma once

#include "object3D.h"

namespace JB
{
	class Group : public Object3D
	{
	public:
		using Ptr = std::shared_ptr<Group>;

		static Group::Ptr create();

		~Group();

	private:
		Group();

	};
}
