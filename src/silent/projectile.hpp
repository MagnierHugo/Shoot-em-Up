// projectile.hpp

#pragma once

#include "Renderable.hpp"
#include "vector.hpp"
#include "audio_manager.hpp"


namespace sl {

    class BasicProjectile : public Component {
    public:
        Vector2 velocity;
        Ref<Renderable> renderable = nullptr;
        Entity* shooter = nullptr;

        void Awake() override;
        void Update(const float deltaTime) override;
        void Init(Entity* shooter, Vector2& direction);
        int Damage;
        void TryDamageEntity(Entity* relevantEntity);
        void PlayHitSound();
        static bool wasInit;
        int audioSourcePoolSize = 100; // if u manage to somehow exceed that u probably already played too much ^^
        int currentAudioSourceIndex = 0;
        Ref<AudioResource> hitSoundResource = nullptr;
    private:
        static int hitCounter;
    };

    class HomingProjectile : public BasicProjectile {
    public:
        void Awake() override;
        void Update(const float deltaTime) override;
    private:
        Ref<Entity> target;
        void SeekTarget();
        float speed = 100.0f;
    };
}