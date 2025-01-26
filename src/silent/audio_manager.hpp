// audio_manager.hpp

#pragma once

#include <iostream>
#include <vector>

#include "resource_manager.hpp"

namespace sl
{
    enum class AudioType
    {
        Sound,
        Music,
    };

    struct AudioResource
    {
        AudioType Type;

        sf::SoundBuffer Buffer;
        sf::Music Music;
    };

    struct AudioSource
    {
        AudioSource* SetAudioResource(Ref<AudioResource> resource)
        {
            AudioResource_ = resource;

            return this;
        }

        Ref<AudioResource> AudioResource_;

        sf::Sound sound;
        sf::Music* music = nullptr;
        int Volume;

        void Play(bool doLoop, int volume_ = 100)
        {
            switch (AudioResource_->Type)
            {
            case AudioType::Sound:
                sound.setBuffer(AudioResource_->Buffer);
                sound.setLoop(doLoop);
                sound.setVolume(volume_);
                sound.setPitch((float)RandomInt(75, 125) / 100.0f);
                sound.play();
                break;

            case AudioType::Music:
                music = &AudioResource_->Music;
                music->setLoop(doLoop);
                music->setVolume(volume_);
                music->play();
                break;

            default:
                break;
            }
        }

        void Stop()
        {
            switch (AudioResource_->Type)
            {
            case AudioType::Sound:
                sound.stop();
                break;

            case AudioType::Music:
                if (music == nullptr) { return; }
                music->stop();
                break;
            default:
                break;
            }
        }

        void SetVolume(int volume_)
        {
            switch (AudioResource_->Type)
            {
            case AudioType::Sound:
                sound.setVolume(volume_);
                break;

            case AudioType::Music:
                if (music == nullptr) { return; }
                music->setVolume(volume_);
                break;
            default:
                break;
            }
        }void UpdateVolume()
        {
            switch (AudioResource_->Type)
            {
            case AudioType::Sound:
                sound.setVolume(Volume);
                break;

            case AudioType::Music:
                if (music == nullptr) { return; }
                music->setVolume(Volume);
                break;
            default:
                break;
            }
        }

        bool IsPlaying()
        {
            switch (AudioResource_->Type)
            {
            case AudioType::Sound:
                return sound.getStatus() == sf::SoundSource::Playing;

            case AudioType::Music:
                if (music == nullptr) { return false; }
                return music->getStatus() == sf::SoundSource::Playing;
            default:
                return false;
            }
        }
    };

    class AudioManager
    {
    public:
        AudioManager();
        ~AudioManager();

        static const Ref<AudioSource> CreateAudioSource(const std::string& name);
        static const Ref<AudioSource> GetAudioSource(const std::string& name);
        static const Ref<AudioResource> CreateAudioResource(const std::string& path, AudioType audioType);
        static const Ref<AudioResource> GetAudioResource(const std::string& path);
    private:
        std::unordered_map<std::string, Ref<AudioSource>> audioSources;
        std::unordered_map<std::string, Ref<AudioResource>> audioResources;
        static AudioManager* instance;
    };
}