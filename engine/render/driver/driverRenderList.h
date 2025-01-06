#pragma once
#include <memory>
#include "../../objects/renderableObject.h"

namespace JB
{
	struct RenderItem
	{
		RenderableObject::Ptr	renderableObject = nullptr;
		float					z = 0;
	};

	class DriverRenderList
	{
	public:
		using Ptr = std::shared_ptr<DriverRenderList>;

		DriverRenderList();

		~DriverRenderList();

		void init();

		void push(RenderableObject::Ptr renderableObject, float z);

		void clear();

		std::vector<RenderItem> getOpaques();

		std::vector<RenderItem> getTransparents();

	private:
		std::vector<RenderItem> m_opaques;			//存储非透明物体
		std::vector<RenderItem> m_transparents;		//存储透明物体
	};
}
