#include "vectorKeyframeTrack.h"
#include "../interpolant/linearInterpolant.h"

namespace JB
{
	VectorKeyframeTrack::VectorKeyframeTrack(const std::string& name, TrackType trackType, const std::vector<float>& times, const std::vector<float>& values)
		: KeyframeTrack(name, trackType, times, values)
	{

	}

	VectorKeyframeTrack::~VectorKeyframeTrack()
	{
	}

	Interpolant::Ptr VectorKeyframeTrack::makeInterpolant()
	{
		return std::make_shared<LinearInterpolant>(m_times, m_values, getValueSize());
	}
}
