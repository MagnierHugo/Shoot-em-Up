// audio_manager.cpp

#include <algorithm>

#include "audio_manager.hpp"

namespace sl
{

    AudioManager* AudioManager::instance = nullptr;

    AudioManager::AudioManager()
    {
        instance = this;
    }

    AudioManager::~AudioManager()
    {
        for (auto& [_, audioSource] : audioSources)
        {
            audioSource->Stop();
        }

        audioSources.clear();
    }

    const Ref<AudioSource> AudioManager::CreateAudioSource(const std::string& name)
    {
        Ref<AudioSource> added = CreateRef<AudioSource>();

        instance->audioSources[name] = added;

        return added;
    }

    const Ref<AudioSource> AudioManager::GetAudioSource(const std::string& name)
    {
        return instance->audioSources.count(name) == 0 ? nullptr : instance->audioSources[name];
    }
    const Ref<AudioResource> AudioManager::CreateAudioResource(const std::string& path, AudioType audioType)
    {
        Ref<AudioResource> created = CreateRef<AudioResource>();
        created->Type = audioType;
        switch (audioType)
        {
        case AudioType::Sound:
            created->Buffer.loadFromFile(path);
            break;

        case AudioType::Music:
            created->Music.openFromFile(path);
            break;

        default:
            break;
        }

        instance->audioResources[path] = created;

        return created;
    }

    const Ref<AudioResource> AudioManager::GetAudioResource(const std::string& path)
    {
        return instance->audioResources[path];
    }
}