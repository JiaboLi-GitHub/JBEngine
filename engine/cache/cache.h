#pragma once
#include <memory>
#include <unordered_map>
#include "../render/driver/driverAttribute.h"
#include "../render/driver/driverShader.h"
#include "../render/driver/driverTexture.h"
#include "../render/driver/driverGeometry.h"
#include "../render/driver/driverMaterial.h"
#include "../render/driver/driverRenderTarget.h"

namespace JB
{
	template<typename T>
	class Cache
	{
	public:
		Cache();
		
		~Cache();

		void add(const size_t& hash, const std::shared_ptr<T>& object);

		std::shared_ptr<T> get(const size_t& hash);

	private:
		std::unordered_map<size_t, std::shared_ptr<T>> m_cache;
	};

	template<typename T>
	inline Cache<T>::Cache()
	{
	}
	
	template<typename T>
	inline Cache<T>::~Cache()
	{
	}
	
	template<typename T>
	inline void Cache<T>::add(const size_t& hash, const std::shared_ptr<T>& object)
	{
		m_cache.emplace(hash, object);
	}
	
	template<typename T>
	inline std::shared_ptr<T> Cache<T>::get(const size_t& hash)
	{
		if (m_cache.count(hash))
			return m_cache[hash];

		return nullptr;
	}

	class DriverShaderCache : public Cache<DriverShader>
	{
	public:
		static DriverShaderCache& getInstance();

		static size_t hash(std::weak_ptr<Material> material);

	protected:
		DriverShaderCache();

		~DriverShaderCache();
	};

	class DriverAttributeFCache : public Cache<DriverAttributeF>
	{
	public:
		static DriverAttributeFCache& getInstance();

	protected:
		DriverAttributeFCache();

		~DriverAttributeFCache();
	};

	class DriverTextureCache : public Cache<DriverTexture>
	{
	public:
		static DriverTextureCache& getInstance();

	protected:
		DriverTextureCache();

		~DriverTextureCache();
	};

	class DriverGeometryCache : public Cache<DriverGeometry>
	{
	public:
		static DriverGeometryCache& getInstance();

	protected:
		DriverGeometryCache();

		~DriverGeometryCache();
	};

	class DriverMaterialCache : public Cache<DriverMaterial>
	{
	public:
		static DriverMaterialCache& getInstance();

	protected:
		DriverMaterialCache();

		~DriverMaterialCache();
	};
}
