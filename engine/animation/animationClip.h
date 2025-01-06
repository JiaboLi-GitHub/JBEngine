#pragma once

#include "keyframeTracks/keyframeTrack.h"

namespace JB
{
	// 动画片段
	class AnimationClip
	{
	public:
		using Ptr = std::shared_ptr<AnimationClip>;

		AnimationClip(const std::string& name, const float& ticksPerSecond, const float& duration, const std::vector<KeyframeTrack::Ptr>& tracks);

		~AnimationClip();

        std::string getName() const;

        float getTicksPerSecond() const;

        float getDuration() const;

        const std::vector<KeyframeTrack::Ptr>& getTracks() const;

        void setName(const std::string& name);

		void setTicksPerSecond(float ticksPerSecond);

		void setDuration(float duration);

		void setTracks(const std::vector<KeyframeTrack::Ptr>& tracks);

	protected:
		std::string m_name;
		float m_ticksPerSecond = 0.0f;
		float m_duration = 0.0f;						// 本动作包含多少ticks
		std::vector<KeyframeTrack::Ptr> m_tracks;		// 关键帧轨迹集
	};
}

