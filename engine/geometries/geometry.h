#pragma once
#include <string>
#include "attribute.h"

namespace JB
{
	//几何类
	class Geometry
	{
	public:
		using Ptr = std::shared_ptr<Geometry>;

		Geometry();

		~Geometry();

		size_t getId();

		AttributeFMap getAttributeFMap();

		void setAttribute(AttributeType type, AttributeF::Ptr attribute);
		AttributeF::Ptr getAttribute(AttributeType type);

		void setIndex(AttributeUInt32::Ptr index);
		AttributeUInt32::Ptr getIndex();

	private:
		size_t m_id;
		AttributeFMap m_attributeMap;
		AttributeUInt32::Ptr m_index;
	};
}
