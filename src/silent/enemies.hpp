#pragma once

#include "Renderable.hpp"
#include "player.hpp"


#include <imgui/imgui.h>
#include <imgui/imgui-SFML.h>

namespace sl {

    class EnemiesMovement : public Component 
    {
    public:
        Ref<Renderable> renderable;

        void Awake() override;

        void Update(const float deltaTime) override;

        void movements(const float deltaTime);

    private:
        float dx = 0;
        float dy = 1;

        Vector2 movementDirection = Vector2(.0f, 10.0f);
        Vector2 invertedMovementDirection = Vector2(.0f, -10.0f);
        Vector2 velocity = Vector2(.0f,20.0f);
        Vector2 currentPlayerPosition;

        Ref<Entity> playerEntity;
        Ref<PlayerMovement> playerMovement;


        Vector2 playerPosition =  Vector2();
        bool playerHasMoved;
    };
    void InitEnemies(Ref<Entity> player);

    class EnemiesShooter : public Component 
    {

    };
}

