#pragma once

#include <memory>
#include "../../global/constant.h"
#include "../interpolant/interpolant.h"

namespace JB
{
	// 关键帧轨迹
	class KeyframeTrack
	{
	public:
		using Ptr = std::shared_ptr<KeyframeTrack>;

		KeyframeTrack(const std::string& name, TrackType trackType, const std::vector<float>& times, const std::vector<float>& values);

		~KeyframeTrack();

		void setTrackType(TrackType type);

		TrackType getTrackType();

		virtual Interpolant::Ptr makeInterpolant() = 0;

		uint32_t getValueSize() noexcept;

		std::string getName();

	protected:
		std::string m_name;
		TrackType m_trackType;
		std::vector<float> m_times;
		std::vector<float> m_values;
	};
}

