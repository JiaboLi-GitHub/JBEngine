#pragma once

#include "keyframeTrack.h"

namespace JB
{
	// 线性关键帧轨迹
	class VectorKeyframeTrack : public KeyframeTrack
	{
	public:
		using Ptr = std::shared_ptr<VectorKeyframeTrack>;

		VectorKeyframeTrack(const std::string& name, TrackType trackType, const std::vector<float>& times, const std::vector<float>& values);

		~VectorKeyframeTrack();

		Interpolant::Ptr makeInterpolant() override;
	};
}

