#include "textureCube.h"

namespace JB
{
	TextureCube::TextureCube(std::unordered_map<TextureCubeTarget, std::string> filePathMap)
	{
		m_type = TextureType::TextureCube;
		for (const auto& [target, filePath] : filePathMap)
		{
			std::vector<unsigned char> data;
			Texture::loadImage(filePath, data, m_width, m_height);
			m_dataMap.emplace(target, std::move(data));
		}
	}

	TextureCube::~TextureCube()
	{
	}

	void TextureCube::setTextureWrappingR(TextureWrapping wrap)
	{
		m_wrapR = wrap;
	}

	TextureWrapping TextureCube::getTextureWrappingR() const
	{
		return m_wrapR;
	}

	const std::unordered_map<TextureCubeTarget, std::vector<unsigned char>>& TextureCube::getDataMap() const
	{
		return m_dataMap;
	}
}
