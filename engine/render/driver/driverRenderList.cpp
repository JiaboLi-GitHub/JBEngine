#include "driverRenderList.h"

namespace JB
{
	DriverRenderList::DriverRenderList()
	{
	}

	DriverRenderList::~DriverRenderList()
	{
	}

	void DriverRenderList::init()
	{
		m_opaques.clear();
		m_transparents.clear();
	}

	void DriverRenderList::push(RenderableObject::Ptr renderableObject, float z)
	{
		RenderItem item = { renderableObject ,z };

		if (renderableObject->getMaterial()->getBlending())
			m_transparents.push_back(item);
		else
			m_opaques.push_back(item);
	}

	void DriverRenderList::clear()
	{
		m_opaques.clear();
		m_transparents.clear();
	}

	std::vector<RenderItem> DriverRenderList::getOpaques()
	{
		return m_opaques;
	}

	std::vector<RenderItem> DriverRenderList::getTransparents()
	{
		return m_transparents;
	}
}
