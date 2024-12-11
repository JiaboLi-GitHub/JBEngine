#pragma once
#include <unordered_map>
#include "texture.h"

namespace JB
{
	class TextureCube : public Texture
	{
	public:
		using Ptr = std::shared_ptr<TextureCube>;

		TextureCube(std::unordered_map<TextureCubeTarget, std::string> filePathMap);

		~TextureCube();
	
		void setTextureWrappingR(TextureWrapping wrap);

		TextureWrapping getTextureWrappingR() const;

		const std::unordered_map<TextureCubeTarget, std::vector<unsigned char>>& getDataMap() const;

	private:
		TextureWrapping	m_wrapR = TextureWrapping::RepeatWrapping;
		std::unordered_map<TextureCubeTarget, std::vector<unsigned char>> m_dataMap;
	};
}
