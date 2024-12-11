#include "material.h"
#include "../tools/idAllocator.h"

namespace  JB
{
	Material::Material()
	{
		m_id = IDAllocator::getInstance().allocateID();
		m_type = MaterialType::Material;
		m_needsUpdate = true;
		m_blending = false;
	}

	Material::~Material()
	{
		IDAllocator::getInstance().releaseID(m_id);
	}

	size_t Material::getId()
	{
		return m_id;
	}

	MaterialType Material::getMaterialType()
	{
		return m_type;
	}

	bool Material::getNeedsUpdate()
	{
		return m_needsUpdate;
	}

	bool Material::getBlending()
	{
		return m_blending;
	}

	void Material::setNeedsUpdate(bool needsUpdate)
	{
		m_needsUpdate = needsUpdate;
	}
}
