#include "cubeMaterial.h"

namespace JB
{
	CubeMaterial::CubeMaterial()
	{
		m_type = MaterialType::CubeMaterial;
	}
	
	CubeMaterial::~CubeMaterial()
	{
	}

	void CubeMaterial::setTextureCube(const TextureCube::Ptr& textureCube)
	{
		m_textureCube = textureCube;
	}

	Texture::Ptr CubeMaterial::getTexture()
	{
		return m_textureCube;
	}
}
