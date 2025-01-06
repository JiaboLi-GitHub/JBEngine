#include "animationClip.h"

namespace JB
{
	AnimationClip::AnimationClip(const std::string& name, const float& ticksPerSecond, const float& duration, const std::vector<KeyframeTrack::Ptr>& tracks)
	{
		m_name = name;
		m_ticksPerSecond = ticksPerSecond;
		m_duration = duration;
		m_tracks = tracks;
	}

	AnimationClip::~AnimationClip()
	{
	}
	
	std::string AnimationClip::getName() const
	{
		return m_name;
	}
	
	float AnimationClip::getTicksPerSecond() const
	{
		return m_ticksPerSecond;
	}
	
	float AnimationClip::getDuration() const
	{
		return m_duration;
	}
	
	const std::vector<KeyframeTrack::Ptr>& AnimationClip::getTracks() const
	{
		return m_tracks;
	}
	
	void AnimationClip::setName(const std::string& name)
	{
		m_name = name;
	}

	void AnimationClip::setTicksPerSecond(float ticksPerSecond)
	{
		m_ticksPerSecond = ticksPerSecond;
	}
	
	void AnimationClip::setDuration(float duration)
	{
		m_duration = duration;
	}
	
	void AnimationClip::setTracks(const std::vector<KeyframeTrack::Ptr>& tracks)
	{
		m_tracks = tracks;
	}
}
