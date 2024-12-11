#pragma once
#include "../texture/texture2D.h"

namespace JB
{
	class RenderTarget
	{
	public:
		using Ptr = std::shared_ptr<RenderTarget>;

		RenderTarget();

		~RenderTarget();

		void setSize(int width, int height);

		Texture2D::Ptr getColorAttachment();

		Texture2D::Ptr getDepthAttachment();

		int getWidth();

		int getHeight();

		bool getNeedsUpdate();

		bool getUseRbo();

		void setNeedsUpdate(bool needsUpdate);

		void setUseRbo(bool use);

	private:
		bool m_needsUpdate = true;
		int m_width = 0;
		int m_height = 0;
		Texture2D::Ptr m_colorAttachment;
		Texture2D::Ptr m_depthAttachment;
		bool m_useRbo = true;
	};
}
