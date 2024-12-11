#include "geometry.h"

namespace JB
{
	Geometry::Geometry()
	{
		m_id = IDAllocator::getInstance().allocateID();
	}

	Geometry::~Geometry()
	{
		IDAllocator::getInstance().releaseID(m_id);
	}

	size_t Geometry::getId()
	{
		return m_id;
	}

	AttributeFMap Geometry::getAttributeFMap()
	{
		return m_attributeMap;
	}

	void Geometry::setAttribute(AttributeType type, AttributeF::Ptr attribute)
	{
		m_attributeMap.emplace(type, attribute);
	}

	AttributeF::Ptr Geometry::getAttribute(AttributeType type)
	{
		if (m_attributeMap.count(type))
			return m_attributeMap[type];
		return nullptr;
	}

	void Geometry::setIndex(AttributeUInt32::Ptr index)
	{
		m_index = index;
	}

	AttributeUInt32::Ptr Geometry::getIndex()
	{
		return m_index;
	}
}
