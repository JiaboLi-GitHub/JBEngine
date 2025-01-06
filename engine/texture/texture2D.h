#pragma once
#include <memory>
#include "texture.h"

namespace JB
{
	class Texture2D : public Texture
	{
	public:
		using Ptr = std::shared_ptr<Texture2D>;

		Texture2D();

		Texture2D(const std::string& filePath);

		Texture2D(unsigned char* dataIn, int widthIn, int heightIn);

		~Texture2D();

		unsigned char* getData();

	private:
		std::vector<unsigned char>	m_data;
	};
}
