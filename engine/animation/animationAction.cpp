#include "animationAction.h"


namespace JB
{
	AnimationAction::AnimationAction(const AnimationClip::Ptr& clip, const Object3D::Ptr& object)
	{
		m_name = clip->getName();
		m_clip = clip;
		m_root = object;

		auto tracks = clip->getTracks();
		auto nTracks = tracks.size();

		m_interpolants.resize(nTracks);
		m_propertyBindings.resize(nTracks);

		for (uint32_t i = 0; i < nTracks; ++i) 
		{
			m_interpolants[i] = tracks[i]->makeInterpolant();
		}

		for (uint32_t i = 0; i < nTracks; ++i) 
		{
			m_propertyBindings[i] = std::make_shared<PropertyBinding>(m_root, tracks[i]->getName(), tracks[i]->getTrackType(), tracks[i]->getValueSize());
			m_interpolants[i]->setBuffer(static_cast<float*>(m_propertyBindings[i]->getBuffer()));
		}
	}

	AnimationAction::~AnimationAction()
	{
	}
	
	void AnimationAction::update(float deltaTime)
	{
		if (!m_running) 
			return;

		float duration = m_clip->getDuration();
		float ticksPerSecond = m_clip->getTicksPerSecond();

		m_currentTime = fmod(m_currentTime + deltaTime * ticksPerSecond * m_speed, duration);

		for (uint32_t i = 0; i < m_interpolants.size(); ++i) 
		{
			m_interpolants[i]->evaluate(m_currentTime);
		}

		for (uint32_t i = 0; i < m_propertyBindings.size(); ++i) 
		{
			m_propertyBindings[i]->apply();
		}
	}
	
	void AnimationAction::play()
	{
		m_running = true;
	}
	
	void AnimationAction::stop()
	{
		m_running = false;
	}
}
