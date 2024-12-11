#include "renderTarget.h"

namespace JB
{
	RenderTarget::RenderTarget()
	{
		m_colorAttachment = std::make_shared<Texture2D>();

		m_depthAttachment = std::make_shared<Texture2D>();
		m_depthAttachment->setDataType(DataType::Float);
		m_depthAttachment->setTextureFormat(TextureFormat::DepthFormat);
		m_depthAttachment->setTextureFilterMin(TextureFilter::NearestFilter);
		m_depthAttachment->setTextureFilterMag(TextureFilter::NearestFilter);
		m_depthAttachment->setTextureWrappingS(TextureWrapping::ClampToBorder);
		m_depthAttachment->setTextureWrappingT(TextureWrapping::ClampToBorder);
	}

	RenderTarget::~RenderTarget()
	{
	}

	void RenderTarget::setSize(int width, int height)
	{
		if (width == m_width && m_height == height)
			return;

		m_width = width;
		m_height = height;

		m_colorAttachment->setSize(width, height);
		m_depthAttachment->setSize(width, height);

		m_needsUpdate = true;
	}

	Texture2D::Ptr RenderTarget::getColorAttachment()
	{
		return m_colorAttachment;
	}

	Texture2D::Ptr RenderTarget::getDepthAttachment()
	{
		return m_depthAttachment;
	}

	int RenderTarget::getWidth()
	{
		return m_width;
	}

	int RenderTarget::getHeight()
	{
		return m_height;
	}

	bool RenderTarget::getNeedsUpdate()
	{
		return m_needsUpdate;
	}

	bool RenderTarget::getUseRbo()
	{
		return m_useRbo;
	}
	
	void RenderTarget::setNeedsUpdate(bool needsUpdate)
	{
		m_needsUpdate;
	}

	void RenderTarget::setUseRbo(bool use)
	{
		m_useRbo = use;
	}
}
