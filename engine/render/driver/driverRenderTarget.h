#pragma once
#include "driverTexture.h"
#include "../renderTarget.h"

namespace JB
{
	class DriverRenderTarget
	{
	public:
		using Ptr = std::shared_ptr<DriverRenderTarget>;

		DriverRenderTarget(std::weak_ptr<RenderTarget> renderTarget);

		~DriverRenderTarget();

		void use();

		void nonUse();

		DriverTexture::Ptr getDriverDepthAttachment();

	private:
		void update();

	private:
		GLuint m_handle = 0;	//FBO
		GLuint m_rbo = 0;	//渲染缓冲对象
		std::weak_ptr<RenderTarget> m_renderTarget;
		DriverTexture::Ptr m_driverColorAttachment = nullptr;
		DriverTexture::Ptr m_driverDepthAttachment = nullptr;
	};
}

