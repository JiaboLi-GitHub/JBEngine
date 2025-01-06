#include "cache.h"
#include <string>

namespace JB
{
	DriverShaderCache& DriverShaderCache::getInstance()
	{
		static DriverShaderCache cache;
		return cache;
	}

	size_t DriverShaderCache::hash(std::weak_ptr<Material> material)
	{
		auto materialPtr = material.lock();
		if (!materialPtr)
			return 0;

		std::string str;
		str += std::to_string(static_cast<uint8_t>(materialPtr->getMaterialType()));

		return std::hash<std::string>()(str);
	}

	DriverShaderCache::DriverShaderCache()
	{
	}

	DriverShaderCache::~DriverShaderCache()
	{
	}

	DriverAttributeFCache& DriverAttributeFCache::getInstance()
	{
		static DriverAttributeFCache cache;
		return cache;
	}

	DriverAttributeFCache::DriverAttributeFCache()
	{
	}

	DriverAttributeFCache::~DriverAttributeFCache()
	{
	}

	DriverTextureCache& DriverTextureCache::getInstance()
	{
		static DriverTextureCache cache;
		return cache;
	}

	DriverTextureCache::DriverTextureCache()
	{
	}

	DriverTextureCache::~DriverTextureCache()
	{
	}

	DriverGeometryCache& DriverGeometryCache::getInstance()
	{
		static DriverGeometryCache cache;
		return cache;
	}

	DriverGeometryCache::DriverGeometryCache()
	{
	}

	DriverGeometryCache::~DriverGeometryCache()
	{
	}

	DriverMaterialCache& DriverMaterialCache::getInstance()
	{
		static DriverMaterialCache cache;
		return cache;
	}

	DriverMaterialCache::DriverMaterialCache()
	{
	}

	DriverMaterialCache::~DriverMaterialCache()
	{
	}
}
