#include "driverGeometry.h"
#include "../../cache/cache.h"

namespace JB
{
	DriverGeometry::Ptr DriverGeometry::getOrAdd(std::weak_ptr<Geometry> geometry)
	{
		auto geometryPtr = geometry.lock();
		if (!geometryPtr)
			return nullptr;

		auto hash = geometryPtr->getId();
		auto driverGeometry = DriverGeometryCache::getInstance().get(hash);
		if (!driverGeometry)
		{
			driverGeometry = std::make_shared<DriverGeometry>(geometry);
			DriverGeometryCache::getInstance().add(hash, driverGeometry);
		}
		return driverGeometry;
	}

	DriverGeometry::DriverGeometry(std::weak_ptr<Geometry> geometry)
	{
		glGenVertexArrays(1, &m_handle);
		m_geometry = geometry;

		if (geometry.expired())
			return;

		Geometry::Ptr geometryPtr = geometry.lock();
		for (const auto& [attributeType, attributeFPtr] : geometryPtr->getAttributeFMap())
		{
			auto driverAttributeFPtr = DriverAttributeFCache::getInstance().get(attributeFPtr->getId());
			if (!driverAttributeFPtr)
			{
				driverAttributeFPtr = std::make_shared<DriverAttributeF>(attributeFPtr, BufferType::ArrayBuffer);
				DriverAttributeFCache::getInstance().add(attributeFPtr->getId(), driverAttributeFPtr);
			}

			m_driverAttributeMap.emplace(attributeType, driverAttributeFPtr);
		}

		m_driverIndex = std::make_shared<DriverAttributeUInt32>(geometryPtr->getIndex(), BufferType::IndexBuffer);
	}

	DriverGeometry::~DriverGeometry()
	{
		if(m_handle)
			glDeleteVertexArrays(1, &m_handle);
	}

	void DriverGeometry::use()
	{
		glBindVertexArray(m_handle);

		update();
		
		auto attribute = m_driverIndex->getAttribute().lock();
		if (attribute)
			m_driverIndex->use();
	}

	void DriverGeometry::nonUse()
	{
		glBindVertexArray(0);

		auto attribute = m_driverIndex->getAttribute().lock();
		if (attribute)
			m_driverIndex->nonUse();
	}

	void DriverGeometry::update()
	{
		for (auto& [attributeType, driverAttributeF] : m_driverAttributeMap)
		{
			auto attribute = driverAttributeF->getAttribute();
			auto attributePtr = attribute.lock();
			if (!attributePtr)
				continue;

			if (!attributePtr->getNeedsUpdate())
				continue;

			auto location = toLocation(attributeType);
			auto itemSize = attributePtr->getItemSize();
			auto dataType = attributePtr->getDataType();
			auto dataTypeSize = toSize(dataType);

			driverAttributeF->use();
			glEnableVertexAttribArray(location);
			glVertexAttribPointer(location, itemSize, toGL(dataType), false, itemSize * dataTypeSize, (void*)0);
			driverAttributeF->nonUse();
		}
	}
}
