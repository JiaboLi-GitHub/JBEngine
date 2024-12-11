#pragma once
#include "material.h"
#include "../texture/textureCube.h"

namespace JB
{
	class CubeMaterial : public Material
	{
	public:
		using Ptr = std::shared_ptr<CubeMaterial>;

		CubeMaterial();

		~CubeMaterial();

		void setTextureCube(const TextureCube::Ptr& textureCube);

		Texture::Ptr getTexture();

	private:
		TextureCube::Ptr m_textureCube = nullptr;
	};
}
