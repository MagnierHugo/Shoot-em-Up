// projectile.cpp

#include <algorithm>
#include "projectile.hpp"
#include "Renderable.hpp"
#include "common.hpp"
#include "idamageable.hpp"
#include "renderer.hpp"
#include "scene.hpp"


namespace sl {

    int BasicProjectile::hitCounter = 0;
    bool BasicProjectile::wasInit = false;

    void BasicProjectile::Awake()
    {
        if (wasInit) { return; }

        hitSoundResource = AudioManager::CreateAudioResource("assets/music/hit.wav", AudioType::Sound);

        for (int i = 0; i < audioSourcePoolSize; i++) // better safe than sorry
        {
            AudioManager::CreateAudioSource("Hit" + std::to_string(i))->SetAudioResource(hitSoundResource);
        }

        wasInit = true;
    }

    void BasicProjectile::Update(const float deltaTime) {
        Transform()->Position += velocity * deltaTime;

        Vector2 size = renderable->GetSize();
        Vector2 position = Transform()->Position;

        if (position.x - size.x <= .0f || position.x >= RenderSettings::ScreenWidth || position.y - size.y <= .0f || position.y >= RenderSettings::ScreenHeight) {
            Owner->scene->RemoveEntity(Owner->uuid);
        }
    }

    void BasicProjectile::Init(Entity* shooter, Vector2& direction)
    {
        Ref<Renderable> renderable = Owner->AddComponent<Renderable>();
        // renderable->ShapeType = ShapeType::Sprite;
        // renderable->Texture = manager.Get("Assets/...", ResourceType::Texture);

        renderable->SetModeRect(Vector2(10.0f, 10.0f), sf::Color::Green);

        Ref<Renderable> shooter_size = shooter->GetComponent<Renderable>();
        Owner->transform->Position = { shooter->transform->Position.x + shooter_size->GetWidth(), shooter->transform->Position.y + shooter_size->GetHeight() / 2 };

        Ref<BasicProjectile> projectile_behavior = Owner->AddComponent<BasicProjectile>();
        projectile_behavior->velocity = direction;
        projectile_behavior->renderable = renderable;
    }

    void BasicProjectile::TryDamageEntity(Entity* relevantEntity)
    {
        if (relevantEntity == shooter) { return; }

        Ref<IDamageable> iDamageablePtr = nullptr;
        if (!relevantEntity->TryGetComponent<IDamageable>(iDamageablePtr)) { return; }

        iDamageablePtr->TakeDamage(Damage);

        PlayHitSound();

        Owner->scene->RemoveEntity(Owner->uuid);
    }

    void BasicProjectile::PlayHitSound()
    {
            for (int iteration = 0; iteration < 100; iteration++) // this loop ensures that we only do <audioSourcePoolSize> loops
            {
                currentAudioSourceIndex = ++currentAudioSourceIndex % audioSourcePoolSize; // while this index ensures that we continue where we left of and remain within the boundaries of the pool

                Ref<AudioSource> currentAudioSource = AudioManager::GetAudioSource("Hit" + std::to_string(currentAudioSourceIndex));
                if (!currentAudioSource->IsPlaying())
                {
                    currentAudioSource->Play(false, 100);
                    return;
                }
            }

            // if reached this point there wasn t enough channels to play all the sounds concurrently so we allocate another one
            std::cout << "Added an audio source for Hit" << std::endl;
            AudioManager::CreateAudioSource("Hit" + std::to_string(audioSourcePoolSize++))->SetAudioResource(hitSoundResource)->Play(false, 100);
    }

    void HomingProjectile::Awake()
    {
        if (wasInit) { return; }

        hitSoundResource = AudioManager::CreateAudioResource("assets/music/hit.wav", AudioType::Sound);

        for (int i = 0; i < audioSourcePoolSize; i++) // better safe than sorry
        {
            AudioManager::CreateAudioSource("Hit" + std::to_string(i))->SetAudioResource(hitSoundResource);
        }

        wasInit = true;

        SeekTarget(); // idec that it doesn t always reach as it willl be called in the update anyway
    }

    void HomingProjectile::Update(const float deltaTime)
    {
        if (target == nullptr)
        {
            SeekTarget();
        }
        else
        {
            if (target->scene == nullptr) // scenePtr is erased when an entity dies (meaning that target was the last (or one of the lasts) reference to it effectively keeping it alive
            {
                target = nullptr;
            }
            else
            {
                velocity = (target->transform->Position - Transform()->Position).NormalizedIfNonZero();
            }
        }

        Transform()->Position += velocity * deltaTime * speed;

        Vector2 size = renderable->GetSize();
        Vector2 position = Transform()->Position;

        if (position.x - size.x <= .0f || position.x >= RenderSettings::ScreenWidth || position.y - size.y <= .0f || position.y >= RenderSettings::ScreenHeight) {
            Owner->scene->RemoveEntity(Owner->uuid);
        }
    }

    void HomingProjectile::SeekTarget()
    {
        target = Owner->scene->GetEntity("Naughty Boy");
    }
}