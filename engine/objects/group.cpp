#include "group.h"

namespace JB
{
	Group::Group()
	{
		m_type = Object3DType::Group;
	}

	Group::Ptr Group::create()
	{
		return std::shared_ptr<Group>(new Group());
	}

	Group::~Group()
	{
	}
}
