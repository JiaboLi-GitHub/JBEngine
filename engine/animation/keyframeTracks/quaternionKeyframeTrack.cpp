#include "quaternionKeyframeTrack.h"

#include "../interpolant/quaternionInterpolant.h"

namespace JB
{
	QuaternionKeyframeTrack::QuaternionKeyframeTrack(const std::string& name, TrackType trackType, const std::vector<float>& times, const std::vector<float>& values)
		: KeyframeTrack(name, trackType, times, values)
	{
	}

	QuaternionKeyframeTrack::~QuaternionKeyframeTrack()
	{
	}

	Interpolant::Ptr QuaternionKeyframeTrack::makeInterpolant()
	{
		return std::make_shared<QuaternionInterpolant>(m_times, m_values, getValueSize());
	}
}
