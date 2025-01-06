#pragma once
#include "driverAttribute.h"
#include "../../geometries/geometry.h"

namespace JB
{
	class DriverGeometry
	{
	public:
		using Ptr = std::shared_ptr<DriverGeometry>;
		
		static DriverGeometry::Ptr getOrAdd(std::weak_ptr<Geometry> geometry);

		DriverGeometry(std::weak_ptr<Geometry> geometry);
		
		~DriverGeometry();

		void use();

		void nonUse();

	private:
		void update();

	private:
		GLuint m_handle;	//VAO
		std::weak_ptr<Geometry> m_geometry;
		DriverAttributeFMap m_driverAttributeMap;
		DriverAttributeUInt32::Ptr m_driverIndex;
	};
}
