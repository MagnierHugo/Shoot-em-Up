#include "player.hpp"
#include "input_manager.hpp"
#include "LightSource.hpp"
#include "projectile.hpp"
#include "time.hpp"
#include "scene.hpp"
#include "renderable.hpp"
#include "renderer.hpp"
#include "idamageable.hpp"
#include "game_manager.hpp"
#include "load_on_input.hpp"


namespace sl {

    void PlayerMovement::Awake()
    {
        renderable = Owner->GetComponent<Renderable>();
        speed = 2.5f;
        width = renderable->GetWidth();
        height = renderable->GetHeight();
    }

    void PlayerMovement::Update(const float deltaTime) {

        Transform()->Position += Vector2(
            InputManager::GetKey(sf::Keyboard::D) - InputManager::GetKey(sf::Keyboard::Q),
            InputManager::GetKey(sf::Keyboard::S) - InputManager::GetKey(sf::Keyboard::Z)
        ).NormalizedIfNonZero() * 144 * deltaTime * speed;

        
        float currentX = Transform()->Position.x;

        Transform()->Position.x = currentX < .0f ? .0f : currentX + width > RenderSettings::ScreenWidth ? RenderSettings::ScreenWidth - width : currentX;

        float currentY = Transform()->Position.y;

        Transform()->Position.y = currentY < bounds.x ? bounds.x : currentY + height > bounds.y ? bounds.y - height : currentY;

        Component::Update(deltaTime);
    }

    void PlayerShooter::Awake()
    {
        ActiveShooter::Awake();

        time_between_shots = 300;
        Damage = 1;
    }

    void PlayerShooter::Update(const float deltaTime) {

        if (!can_shoot) { return; }

        if (InputManager::GetKey(sf::Keyboard::Space))
        {
            ShootBasicProjectile(Vector2(700, 0));
            return;
        }

        if (InputManager::GetKey(sf::Keyboard::LAlt))
        {
            ShootHomingProjectile(Vector2::Right.Copy());
        }
    }
    
    void PlayerShooter::BombBonus() {
        Damage *= 2;
        Time::AddTimer(5000, [&]() { Damage /= 2; });
    }

    void PlayerShooter::AttackSpeedBonus() {
        time_between_shots /= 2;
        Time::AddTimer(3000, [&]() { time_between_shots *= 2; });
    }

    void InitPlayer(Ref<Entity> player) {
        Ref<Renderable> renderablePtr = player->AddComponent<Renderable>();
        renderablePtr->SetModeSprite(sl::ResourceManager::Get("assets/Perso_Principal/Personnage_principale.png", sl::ResourceType::Texture));

        player->transform->Scale = Vector2::One * .1875f; // smol
        player->transform->Position = Vector2(200, sl::RenderSettings::ScreenHeight / 2 + renderablePtr->GetHeight() / 2);
        player->has_collider = true;

        { auto _ = player->AddComponent<PlayerMovement>(); } // NOTE(hugo): Discard var and reuse _ as discard variable name; 
        { auto _ = player->AddComponent<PlayerShooter>(); }
        { auto _ = player->AddComponent<LightSource>(); }
        { auto _ = player->AddComponent<GameManager>(); }
        
        Ref<LoadSceneOnInput> pause_scene_loader = player->AddComponent<LoadSceneOnInput>();
        pause_scene_loader->Init("Pause", sf::Keyboard::Key::Escape);

        Ref<IDamageable> iDamageablePtr = player->AddComponent<IDamageable>();
        iDamageablePtr->SetMaxHealth(5);
        Scene* scene = player->scene;
        iDamageablePtr->onHit += [scene](int health) {
            scene->GetEntity("Life Sprite " + std::to_string(health))->GetComponent<Renderable>()->SetModeSprite(ResourceManager::Get("assets/UI-UX/UI/roses_vie_perdu.png", ResourceType::Texture));
        };
        iDamageablePtr->onDeath += []() { std::cout << "lowkey dead" << std::endl; };
        iDamageablePtr->onDeath += []() { ScenesManager::LoadScene("GameOver"); /*WARP_TO_BACKGROUND*/ };
    }
}
