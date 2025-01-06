#pragma once

#include "propertyBinding.h"
#include "AnimationClip.h"
#include "interpolant/interpolant.h"
#include "keyframeTracks/keyframeTrack.h"

namespace JB
{
	class AnimationAction
	{
	public:
		using Ptr = std::shared_ptr<AnimationAction>;

		AnimationAction(const AnimationClip::Ptr& clip, const Object3D::Ptr& object);

		~AnimationAction();

		void update(float deltaTime);

		void play();

		void stop();

	public:
		std::string m_name;											//动画名称
		float m_speed = 1.0f;										//播放速度
		bool m_running = false;										//动画是否处于播放状态
		float m_currentTime = 0.0f;									//用ticks来计数的当前动画播放到了第几个ticks
		AnimationClip::Ptr	m_clip = nullptr;
		Object3D::Ptr		m_root = nullptr;						//当前读取模型最后结果AssimpResult里面的mObject
		std::vector<PropertyBinding::Ptr>	m_propertyBindings;		//每个插值器对应的PropertyBindings
		std::vector<Interpolant::Ptr>		m_interpolants;			//当前mClip里面的每个KeyFrameTrack所对应的插值器集合
	};
}
