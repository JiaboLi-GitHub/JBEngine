#include "driverMaterial.h"
#include "../../cache/cache.h"

namespace JB
{
	DriverMaterial::Ptr DriverMaterial::getOrAdd(std::weak_ptr<Material> material)
	{
		auto materialPtr = material.lock();
		if (!materialPtr)
			return nullptr;

		auto hash = materialPtr->getId();
		auto driverMaterial = DriverMaterialCache::getInstance().get(hash);
		if (!driverMaterial)
		{
			driverMaterial = std::make_shared<DriverMaterial>(material);
			DriverMaterialCache::getInstance().add(hash, driverMaterial);
		}
		return driverMaterial;
	}

	DriverMaterial::DriverMaterial(std::weak_ptr<Material> material)
	{
		m_material = material;

		update();
	}

	DriverMaterial::~DriverMaterial()
	{
	}

	void DriverMaterial::use()
	{
		auto materialPtr = m_material.lock();
		if (!materialPtr)
			return;

		update();

		switch (materialPtr->getMaterialType())
		{
		case MaterialType::BaseMaterial:
		{
			if(m_texture)
				m_texture->use();
			break;
		}
		case MaterialType::CubeMaterial:
		{
			if (m_texture)
				m_texture->use();
			break;
		}
		case MaterialType::PhongLightingMaterial:
		{
			if (m_diffuseTexture)
				m_diffuseTexture->use();
			if (m_specularTexture)
				m_specularTexture->use();
			break;
		}
		default:
			break;
		}
	}

	void DriverMaterial::nonUse()
	{
		auto materialPtr = m_material.lock();
		if (!materialPtr)
			return;

		switch (materialPtr->getMaterialType())
		{
		case MaterialType::BaseMaterial:
		{
			if(m_texture)
				m_texture->nonUse();
			break;
		}
		case MaterialType::CubeMaterial:
		{
			if (m_texture)
				m_texture->nonUse();
			break;
		}
		case MaterialType::PhongLightingMaterial:
		{
			if (m_diffuseTexture)
				m_diffuseTexture->nonUse();
			if (m_specularTexture)
				m_specularTexture->nonUse();
			break;
		}
		default:
			break;
		}
	}

	void DriverMaterial::update()
	{
		auto materialPtr = m_material.lock();
		if (!materialPtr)
			return;

		switch (materialPtr->getMaterialType())
		{
		case MaterialType::BaseMaterial:
		{
			auto baseMaterial = std::dynamic_pointer_cast<BaseMaterial>(materialPtr);
			auto texture = baseMaterial->getTexture();
			m_texture = DriverTexture::getOrAdd(texture);
			break;
		}
		case MaterialType::CubeMaterial:
		{
			auto cubeMaterial = std::dynamic_pointer_cast<CubeMaterial>(materialPtr);
			auto texture = cubeMaterial->getTexture();
			m_texture = DriverTexture::getOrAdd(texture);
			break;
		}
		case MaterialType::PhongLightingMaterial:
		{
			auto phongLightingMaterial = std::dynamic_pointer_cast<PhongLightingMaterial>(materialPtr);
			if (phongLightingMaterial->getDiffuse())
			{
				m_diffuseTexture = DriverTexture::getOrAdd(phongLightingMaterial->getDiffuse());
			}
			if (phongLightingMaterial->getSpecular())
			{
				m_specularTexture = DriverTexture::getOrAdd(phongLightingMaterial->getSpecular());
			}
			break;
		}
		default:
			break;
		}
	}
}
