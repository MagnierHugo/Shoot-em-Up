#include "enemies.hpp"
#include "scene.hpp"
#include "renderer.hpp"
#include "player.hpp"
#include "projectile.hpp"
#include "input_manager.hpp"

namespace sl {
    void EnemiesMovement::Awake()
    {
        renderable = Owner->GetComponent<Renderable>();
        playerMovement = Owner->scene->GetEntity("Player")->GetComponent<PlayerMovement>();
    }

    void EnemiesMovement::Update(const float deltaTime)
    {
        static sl::Vector2 previousPlayerPosition = Transform()->Position;
        sl::Vector2 currentPlayerPosition = Transform()->Position;

        movements(deltaTime);

        float width = renderable->GetWidth();
        float currentX = Transform()->Position.x;
        Transform()->Position.x = currentX < .0f ? .0f : currentX + width > RenderSettings::ScreenWidth ? RenderSettings::ScreenWidth - width : currentX;

        float height = renderable->GetHeight();
        float currentY = Transform()->Position.y;
        Transform()->Position.y = currentY < .0f ? .0f : currentY + height > RenderSettings::ScreenHeight ? RenderSettings::ScreenHeight - height : currentY;
       
    }
    void EnemiesMovement::movements(const float deltaTime)
    {
        //Stalking
        sl::Vector2 currentPosition = Transform()->Position;

        if (currentPosition.y >= playerMovement->Transform()->Position.y) 
        {
            Transform()->Position.y += (playerMovement->Transform()->Position.y - currentPosition.y) * deltaTime * 144;
        }

        else if (currentPosition.y < playerMovement->Transform()->Position.y) 
        {
            Transform()->Position.y -= (currentPosition.y - playerMovement->Transform()->Position.y) * deltaTime * 144;
        }
    }

    void InitEnemies(Ref<Entity> enemy)
    {
        sl::Ref<Renderable> renderablePtr;
        renderablePtr = enemy->AddComponent<Renderable>();
        renderablePtr->SetModeSprite(ResourceManager::Get("assets/Ennemis/ENNEMI_MAFIA/ennemi_mafia_car2.png", ResourceType::Texture));

        enemy->transform->Scale = sl::Vector2(0.1f, 0.2f);
        enemy->transform->Position = sl::Vector2(700, 350);


        { auto _ = enemy->AddComponent<EnemiesMovement>(); } // NOTE(Ivania): just stole hugo's code; 
    }

}