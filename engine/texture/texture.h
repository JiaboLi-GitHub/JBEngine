#pragma once
#include <memory>
#include "../global/constant.h"

namespace JB
{
	class Texture
	{
	public:
		using Ptr = std::shared_ptr<Texture>;

		virtual ~Texture();

		size_t getId();

		TextureType getType();

		int getWidth();

		int getHeight();

		bool getNeedsUpdate();

		void setNeedsUpdate(bool needsUpdate);

		void setSize(int width, int height);

		void setTextureWrappingS(TextureWrapping wrap);

		void setTextureWrappingT(TextureWrapping wrap);
		
		void setTextureFilterMin(TextureFilter filter);

		void setTextureFilterMag(TextureFilter filter);

		void setTextureFormat(TextureFormat format);
		
		void setDataType(DataType dataType);

		void setEdgeColor(float r, float g, float b, float a);

		void setEdgeColor(const glm::vec4& color);

		TextureWrapping getTextureWrappingS() const;

		TextureWrapping getTextureWrappingT() const;

		TextureFilter getTextureFilterMin() const;

		TextureFilter getTextureFilterMag() const;

		TextureFormat getTextureFormat();

		DataType getDataType();

		glm::vec4 getEdgeColor() const;

	protected:
		Texture();
		
		static void loadImage(const std::string filePath, std::vector<unsigned char>& data, int& width, int& height);

	protected:
		size_t m_id;
		bool m_needsUpdate = true;
		TextureType m_type;
		int m_width = 0;
		int m_height = 0;
		TextureWrapping	m_wrapS = TextureWrapping::RepeatWrapping;
		TextureWrapping	m_wrapT = TextureWrapping::RepeatWrapping;
		TextureFilter m_minFilter = TextureFilter::LinearFilter;
		TextureFilter m_magFilter = TextureFilter::LinearFilter;
		TextureFormat m_format;		//纹理格式
		DataType m_dataType;
		glm::vec4 m_edgeColor;
	};
}
