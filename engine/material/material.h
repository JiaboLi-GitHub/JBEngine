#pragma once
#include "memory"
#include "../global/constant.h"
#include "../texture/texture.h"

namespace JB
{
	class Material
	{
	public:
		using Ptr = std::shared_ptr<Material>;

		virtual ~Material();
		
		size_t getId();

		MaterialType getMaterialType();

		bool getNeedsUpdate();

		bool getUseIndexDraw();

		bool getBlending();

		void setNeedsUpdate(bool needsUpdate);

	protected:
		Material();

	protected:
		size_t m_id;
		bool m_needsUpdate;
		MaterialType m_type;
		bool m_blending;		//是否启动混合
	};
}
