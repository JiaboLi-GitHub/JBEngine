#include "baseMaterial.h"

namespace JB
{
	BaseMaterial::BaseMaterial()
	{
		m_type = MaterialType::BaseMaterial;
	}

	BaseMaterial::~BaseMaterial()
	{
	}

	void BaseMaterial::setTexture(const Texture2D::Ptr& texture)
	{
		m_texture = texture;
	}

	Texture::Ptr BaseMaterial::getTexture()
	{
		return m_texture;
	}
}
