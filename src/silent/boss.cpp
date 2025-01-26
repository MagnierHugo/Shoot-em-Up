// boss.cpp

#define _USE_MATH_DEFINES

#include <cstdlib>
#include <cmath>
#include <limits.h>

#define _2_M_PI (M_PI * 2)

#include "boss.hpp"
#include "renderer.hpp"
#include "idamageable.hpp"
#include "Camera.hpp"
#include "time.hpp"


namespace sl {

    void BossMovement::Awake() {
        renderable = Owner->GetComponent<Renderable>();
        Owner->GetComponent<BossShooter>()->Disable();

        current_pattern = PatternTypes::Start;
        base_position = { RenderSettings::ScreenWidth - renderable->GetWidth(), (RenderSettings::ScreenHeight - renderable->GetHeight()) / 2 };
        speed = 100.0f;

        patterns[PatternTypes::UpDown] = MovementPattern({ { base_position.x, bounds.x }, { base_position.x, bounds.y - renderable->GetHeight() }, base_position });
        patterns[PatternTypes::Circle] = MovementPattern(GetElipsePoints(bounds.y - bounds.x));
        patterns[PatternTypes::Dash] = MovementPattern({ {0 - renderable->GetWidth(), base_position.y } });
        patterns[PatternTypes::Start] = MovementPattern({base_position});
    }

    void BossMovement::Update(const float deltaTime) {
        if (current_pattern != PatternTypes::Idling) {
            if (patterns[current_pattern].Move(Transform()->Position, speed * deltaTime * (current_pattern == PatternTypes::Dash ? 5 : 1))) { return; }

            Transform()->Position = base_position;
            ChangePattern();
            // current_patern = PaternTypes::Circle;
        }
        else if (!idling) {
            ChangePattern();
            // current_patern = PaternTypes::Circle;
        }
    }

    void BossMovement::ChangePattern() {
        if (current_pattern == PatternTypes::Start) { Owner->GetComponent<BossShooter>()->Enable(); }
        int rdm = RandomInt(0, static_cast<i32>(PatternTypes::Count) - 1);
        //std::cout << rdm << std::endl;
        current_pattern = static_cast<PatternTypes>(rdm);
        if (!(current_pattern == PatternTypes::Idling)) { return; }

        idling = true;
        StartTimer(3000, [&]() { this->idling = false; });
    }
    
    std::vector<Vector2> BossMovement::GetElipsePoints(float road_heigth) const {
        u32 points_count = 30;
        Vector2 radius = { renderable->GetWidth() / 2, (road_heigth - renderable->GetHeight()) / 2 };
        std::vector<Vector2> points;

        for (u32 i = 1; i < points_count; i++) {
            float angle = i * _2_M_PI / points_count;
            float x = std::cos(angle) * radius.x;
            float y = std::sin(angle) * radius.y;

            points.push_back(Vector2(x - radius.x, y) + base_position);
        }

        points.push_back(base_position);
        return points;
    }

    b8 MovementPattern::Move(Vector2& position, float length) {
        Vector2 distance = path[index] - position;
        if (distance.Length() < length) {
            if (index == path.size() - 1) { 
                index = 0;
                return false; 
            }
            index++;
            distance = path[index] - position;
        }
        position += distance.Normalized() * length;
        return true;
    }
    
    void BossShooter::Awake() {
        renderable = Owner->GetComponent<Renderable>();

        attack_type = AttackTypes::Burst;
        total_bullets_left = 80;
        bullets_per_mags = 20;
        time_between_shots = 100;
        time_to_reload = 1000;
        reloading = false;

        attack_patterns[AttackTypes::Shotgun] = { Vector2::Left, Vector2::Left.Rotated(7.5f), Vector2::Left.Rotated(15.0f), Vector2::Left.Rotated(-7.5f), Vector2::Left.Rotated(-15.0f) };
        attack_patterns[AttackTypes::Burst] = { Vector2::Left };
        attack_patterns[AttackTypes::Mortar] = { Vector2::One };
    }

    void BossShooter::Update(const float deltaTime) {
        if (reloading || !can_shoot) { return; }

        float angle;

        switch (attack_type)
        {
        case AttackTypes::Burst:
            ShootBasicProjectile(attack_patterns[attack_type][0] * 600);            
            break;

        case AttackTypes::Shotgun:
            angle = 3.75f * std::cos((total_bullets_left % bullets_per_mags) * M_PI / 2);
            for (auto& it : attack_patterns[attack_type]) {
                ShootBasicProjectile(it.Rotated(angle) * 600, true);
            }
            StartShootingCooldown();
            break;

        case AttackTypes::Mortar:
            break;
        
        default:
            break;
        }

        total_bullets_left--;

        if (total_bullets_left == 0) {
            u32 weapon_switch_time = time_to_reload;
            SwitchAttackType();
            weapon_switch_time += time_to_reload; // the new one
            reloading = true;
            StartTimer(weapon_switch_time,  [&]() { this->reloading = false; });
        }
        else if ((total_bullets_left % bullets_per_mags) == 0) {
            reloading = true;
            StartTimer(time_to_reload, [&]() { this->reloading = false; });
        }

    }
    
    void BossShooter::SwitchAttackType() {
        attack_type = static_cast<AttackTypes>(RandomInt(0, static_cast<i32>(AttackTypes::Count) - 1));
        switch (attack_type)
        {
        case AttackTypes::Burst:
            total_bullets_left = 100;
            bullets_per_mags = 20;
            time_between_shots = 150;
            time_to_reload = 1000;
            break;

        case AttackTypes::Shotgun:
            total_bullets_left = 15;
            bullets_per_mags = 3;
            time_between_shots = 300;
            time_to_reload = 1500;
            break;

        case AttackTypes::Mortar:
            break;
        
        default:
            break;
        }
    }
    
    void InitBoss(Ref<Entity> boss, int currentDifficulty) {
        Ref<Renderable> renderable = boss->AddComponent<Renderable>();
        renderable->SetModeSprite(ResourceManager::Get(ScenesManager::GetCurrentScene() == "Lvl1" ? "assets/sprites/boss.png" : "assets/sprites/damaged_boss.png", ResourceType::Texture));

        boss->transform->Scale = { .25f, .25f };
        boss->transform->Position = { (float)RenderSettings::ScreenWidth * 1.5f, (RenderSettings::ScreenHeight - renderable->GetHeight()) / 2 };
        boss->has_collider = true;

        { auto _ = boss->AddComponent<BossMovement>(); }
        { auto _ = boss->AddComponent<BossShooter>(); }

        Ref<IDamageable> iDamageablePtr = boss->AddComponent<IDamageable>();
        iDamageablePtr->SetMaxHealth(currentDifficulty == 0 ? 27 : currentDifficulty == 1 ? 41 : 64); // most random numbers to ever exist
        iDamageablePtr->onDeath += [=]() { boss->scene->RemoveEntity(boss); };
        iDamageablePtr->onDeath += [=]() { Camera::ApplyScreenShake(25, 25, 300); };
        iDamageablePtr->onDeath += [=]() { Time::AddTimer(3000, []() { ScenesManager::LoadScene("Win"); }); };
    }

}