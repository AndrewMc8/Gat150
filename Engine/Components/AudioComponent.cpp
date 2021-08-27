#include "AudioComponent.h"
#include "Audio/AudioSystem.h"
#include "Engine.h"

namespace nc
{
	void AudioComponent::Update()
	{
		if (playOnAwake && !played)
		{
			played = true;
			Play();
		}
	}

	void AudioComponent::Play()
	{
		channel.Stop();
		channel = owner->scene->engine->Get<AudioSystem>()->PlayAudio(soundNames, volume, pitch, loop);
	}

	void AudioComponent::Stop()
	{
		channel.Stop();
	}

	bool AudioComponent::Write(const rapidjson::Value& value) const
	{
		return false;
	}

	bool AudioComponent::Read(const rapidjson::Value& value)
	{
		//<json read soundName, volume, pitch, loop, playOnAwake>
		JSON_READ(value, soundNames);
		JSON_READ(value, volume);
		JSON_READ(value, pitch);
		JSON_READ(value, loop);
		JSON_READ(value, playOnAwake);

		// add the audio to the audio system if there's a valid soundName string
		if (soundNames != "") owner->scene->engine->Get<AudioSystem>()->AddAudio(soundNames, soundNames);

		return true;
	};
}
