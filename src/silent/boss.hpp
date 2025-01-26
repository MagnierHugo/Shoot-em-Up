// boss.hpp

#pragma once

#include <vector>

#include "Component.hpp"
#include "vector.hpp"
#include "Renderable.hpp"
#include "active_shooter.hpp"


namespace sl {

    class MovementPattern {
    public:
        MovementPattern() = default;
        MovementPattern(const std::vector<Vector2>& _path) : path(_path), index(0) {}

        b8 Move(Vector2& position, float length); // return false if the movement is finished

    private:
        std::vector<Vector2> path;
        u32 index;

    };

    class BossMovement : public Component {
    private:
    
        enum class PatternTypes : u8 {
            Idling, // Doesn't move
            Circle, // Moves in a circular motion
            UpDown, // Moves along the Y-axis
            Dash, // Dash forward trying to hit the player. Warp from the left of the screen to it's original position

            Count, // Always keep at the bottom as it is the number of Values inside the enum
            Start // Only exception to the rule as it will only be used once
        };

    public:
        Vector2 bounds;

        void Awake() override;
        void Update(const float deltaTime) override;

    private:
        Ref<Renderable> renderable;
        std::unordered_map<PatternTypes, MovementPattern> patterns;
        PatternTypes current_pattern;
        Vector2 base_position;
        float speed;
        b8 idling;

        void ChangePattern();
        std::vector<Vector2> GetElipsePoints(float road_height) const;

    };

    class BossShooter : public ActiveShooter {
    private:

        enum class AttackTypes : u8 {
            Burst,
            Shotgun,
            Mortar,

            Count
        };

    public:
        void Awake() override;
        void Update(const float deltaTime) override;

    private:
        Ref<Renderable> renderable;
        AttackTypes attack_type;
        std::unordered_map<AttackTypes, std::vector<Vector2>> attack_patterns;
        u8 total_bullets_left;
        u8 bullets_per_mags;
        u32 time_to_reload;
        b8 reloading;

        void SwitchAttackType();

    };

    void InitBoss(Ref<Entity> boss, int currentDifficulty);

}