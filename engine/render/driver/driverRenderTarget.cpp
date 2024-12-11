#include "driverRenderTarget.h"
#include <iostream>

namespace JB
{
	DriverRenderTarget::DriverRenderTarget(std::weak_ptr<RenderTarget> renderTarget)
	{
		if (!renderTarget.expired())
		{
			m_renderTarget = renderTarget;
			glGenFramebuffers(1, &m_handle);
			glGenRenderbuffers(1, &m_rbo);
		}
		update();
	}

	DriverRenderTarget::~DriverRenderTarget()
	{
		if(m_rbo)
			glDeleteRenderbuffers(1, &m_rbo);

		if(m_handle)
			glDeleteFramebuffers(1, &m_handle);
	}

	void DriverRenderTarget::use()
	{
		update();
		glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
	}

	void DriverRenderTarget::nonUse()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	DriverTexture::Ptr DriverRenderTarget::getDriverDepthAttachment()
	{
		return m_driverDepthAttachment;
	}

	void DriverRenderTarget::update()
	{
		auto renderTargetPtr = m_renderTarget.lock();
		if (!renderTargetPtr)
			return;

		if (!renderTargetPtr->getNeedsUpdate())
			return;

		glBindFramebuffer(GL_FRAMEBUFFER, m_handle);

		m_driverColorAttachment = DriverTexture::getOrAdd(renderTargetPtr->getColorAttachment());
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_driverColorAttachment->getHandle(), 0);

		if (renderTargetPtr->getUseRbo())
		{
			glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, renderTargetPtr->getWidth(), renderTargetPtr->getHeight());
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);
		}
		else
		{
			m_driverDepthAttachment = DriverTexture::getOrAdd(renderTargetPtr->getDepthAttachment());
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_driverDepthAttachment->getHandle(), 0);
		}

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		renderTargetPtr->setNeedsUpdate(false);
	}
}
