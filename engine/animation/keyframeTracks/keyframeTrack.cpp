#include "keyframeTrack.h"

namespace JB
{
	KeyframeTrack::KeyframeTrack(const std::string& name, TrackType trackType, const std::vector<float>& times, const std::vector<float>& values)
	{
		m_name = name;
		m_trackType = trackType;
		m_times = times;
		m_values = values;
	}

	KeyframeTrack::~KeyframeTrack()
	{
	}

	void KeyframeTrack::setTrackType(TrackType type)
	{
		m_trackType = type;
	}

	TrackType KeyframeTrack::getTrackType()
	{
		return m_trackType;
	}
	
	uint32_t KeyframeTrack::getValueSize() noexcept
	{
		return m_values.size() / m_times.size();
	}

	std::string KeyframeTrack::getName()
	{
		return m_name;
	}
}
