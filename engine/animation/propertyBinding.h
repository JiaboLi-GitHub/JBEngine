#pragma once

#include "../objects/object3D.h"

namespace JB
{
	struct ParseNames 
	{
		std::string nodeName;			// 当前绑定的骨骼的名字
		TrackType trackType;			// 轨迹类型
	};

	class PropertyBinding
	{
	public:
		using Ptr = std::shared_ptr<PropertyBinding>;

		PropertyBinding(const Object3D::Ptr& root, const std::string& nodeName, TrackType trackType, const size_t& valueSize);

		~PropertyBinding();

		void apply();

		void* getBuffer();

	private:
		Object3D::Ptr findeNode(const Object3D::Ptr& object, const std::string& nodeName);

	private:
		Object3D::Ptr m_node;
		ParseNames m_parseNames;
		void* m_buffer = nullptr;		
	};
}

