#pragma once

#include "keyframeTrack.h"

namespace JB
{
	// 球面关键帧轨迹
	class QuaternionKeyframeTrack : public KeyframeTrack
	{
	public:
		using Ptr = std::shared_ptr<QuaternionKeyframeTrack>;

		QuaternionKeyframeTrack(const std::string& name, TrackType trackType, const std::vector<float>& times, const std::vector<float>& values);

		~QuaternionKeyframeTrack();

		Interpolant::Ptr makeInterpolant() override;
	};
}

