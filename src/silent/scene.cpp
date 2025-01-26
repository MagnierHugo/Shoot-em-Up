// world.cpp

#include <algorithm>
#include <functional>
//#include <windows.h>
#include <array>

#include "common.hpp"
#include "scene.hpp"
#include "Component.hpp"
#include "Renderable.hpp"
#include "time.hpp"
#include "transform.hpp"
#include "idamageable.hpp"
#include "projectile.hpp"
#include "load_on_input.hpp"
#include "button.hpp"
#include "slider.hpp"
#include "renderer.hpp"
#include "scrollable.hpp"
#include "boss.hpp"
#include "player.hpp"
#include "rescaler.hpp"
#include "mover.hpp"
#include "game_manager.hpp"
#include "audio_manager.hpp"

namespace sl {

    Scene::Scene() : Scene(false) { }

    Scene::Scene(bool activeOnStartup)
    {
        Active = activeOnStartup;
    }

    Scene::~Scene()
    {
        entities.clear();
    }

    Ref<Entity> Scene::AddEntity(const std::string& name)
    {
        Ref<Entity> added = CreateRef<Entity>(this);
    
        added->name = name;

        added->transform = added->AddComponent<Transform>();
        added->collider_callback = [](Ref<Entity> a, Ref<Entity> b) { };

        onAwake += std::bind(&Entity::Awake, added);
        entities.push_back(added);

        return added;
    }

    Ref<Entity> Scene::AddExistingEntity(Ref<Entity> entity)
    {
        entities.push_back(entity);

        return entity; // if u lost it in the process ^^
    }

    Ref<Entity> Scene::GetEntity(const u64& uuid)
    {
        for (auto& it : entities) {
            if (it->uuid == uuid)
                return it;
        }
        
        return nullptr;
    }

    Ref<Entity> Scene::GetEntity(const std::string& name)
    {
        for (auto& it : entities) {
            if (it->name == name)
                return it;
        }

        return nullptr;
    }

    void Scene::RemoveEntity(Ref<Entity> entity)
    {
        entitiesToDiscard.push_back(entity);
    }

    void Scene::RemoveEntity(const u64& uuid) {
        Ref<Entity> entity = GetEntity(uuid);

        if (entity == nullptr) { return; }

        RemoveEntity(entity);
    }

    void Scene::RemoveEntity(const std::string& name) {
        Ref<Entity> entity = GetEntity(name);

        if (entity == nullptr) { return; }

        RemoveEntity(entity);
    }

    void Scene::DiscardEntities()
    {
        if (entitiesToDiscard.empty()) { return; }

        for (Ref<Entity> entity : entitiesToDiscard)
        {
            DiscardEntity(entity);
        }

        entitiesToDiscard.clear();
    }

    void Scene::DiscardEntity(Ref<Entity> entity)
    {
        // std::cout << "Tryna remove: " << entity->name << std::endl;
        if (entity->parent != nullptr)
        {
            entity->parent->childEntities.erase(std::find(entity->parent->childEntities.begin(), entity->parent->childEntities.end(), entity));
        }

        for (int entityIndex = (int)entity->childEntities.size() - 1; entityIndex > -1; entityIndex--)
        {
            entity->childEntities[entityIndex]->parent = nullptr; // to avoid changing children count and mess up loop
            DiscardEntity(entity->childEntities[entityIndex]);
        }

        entity->OnDestroy();
        entities.erase(std::find(entities.begin(), entities.end(), entity));
    }

    void Scene::AddLightSource(Ref<LightSource> newLightSource)
    {
        lightSources.push_back(newLightSource);
    }
    
    void Scene::AddOnAwake(std::function<void(void)> newOnAwake) {
        onAwake.Add(newOnAwake);
    }

    void Scene::Awake()
    {
        onAwake();
    }

    void Scene::Update(const float deltaTime)
    {
        Awake();

        for (Ref<Entity>& entity : entities)
        {
            if (entity == nullptr || entity.get() == nullptr) { continue; } // I don t get how tf nullptr but at least I can do that

            // how the FUCK entity nullptr here ?

            // time for the true war crimes
            //if (reinterpret_cast<void*>(entity.get()) == reinterpret_cast<void*>(0xFFFFFFFFFFFFFFFF)) { std::cout << "Pragmatic solution\n"; continue; }

            // WARNING TO SOCIETY
            // I am not afraid to do
            /*
            bool doSkip;
            for (int i = 0xFFFFFFFFFFFFFFFF; i > -1; i--)
            {
                if (entity.get() == reinterpret_cast<Entity*>(i))
                {
                    doSkip = true;
                    break;
                }
            }
            if (doSkip) { continue; }
            */
            // if that s what it takes
            // YOU should be afraid
            entity->Update(deltaTime);
            /*__try {
                entity->Update(deltaTime);
            }
            __except (1) {
                std::cerr << "Lowkey don t have access ^^: " << entity << std::endl;
            }*/
            
        }

        HandleCollisions();

        DiscardEntities();
    }

    void Scene::HandleCollisions()
    {
        Ref<Renderable> firstEntityRenderablePtr = nullptr;
        Ref<Renderable> secondEntityRenderablePtr = nullptr;
        for (int firstEntityIndex = 0; firstEntityIndex < entities.size(); firstEntityIndex++)
        {
            Ref<Entity> firstEntity = entities[firstEntityIndex];

            if (!firstEntity->has_collider) { continue; }
            if (!firstEntity->TryGetComponent<Renderable>(firstEntityRenderablePtr)) { continue; } // the collider relies on the Renderable component 

            sf::FloatRect it1_shape = firstEntityRenderablePtr->GetBounds();

            for (int secondEntityIndex = firstEntityIndex + 1; secondEntityIndex < entities.size(); secondEntityIndex++)
            {
                Ref<Entity> secondEntity = entities[secondEntityIndex];

                if (!secondEntity->has_collider) { continue; }
                if (!secondEntity->TryGetComponent<Renderable>(secondEntityRenderablePtr)) { continue; }

                sf::FloatRect it2_shape = secondEntityRenderablePtr->GetBounds();

                //std::cout << "Trying: " << firstEntity->name << " | " << secondEntity->name << "\n";

                if (it1_shape.intersects(it2_shape))
                {
                    firstEntity->collider_callback(firstEntity, secondEntity);

                    secondEntity->collider_callback(secondEntity, firstEntity);

                    // nasty but the current GetComponent<> doesn t work with polymophism
                    Ref<BasicProjectile> projectilePtr;
                    Ref<HomingProjectile> homingProjectilePtr;
                    bool foundPlainProj = false;
                    if ((foundPlainProj = firstEntity->TryGetComponent<BasicProjectile>(projectilePtr)) || firstEntity->TryGetComponent<HomingProjectile>(homingProjectilePtr))
                    {
                        if (foundPlainProj)
                        {
                            projectilePtr->TryDamageEntity(secondEntity.get());
                        }
                        else
                        {
                            homingProjectilePtr->TryDamageEntity(secondEntity.get());
                        }
                    }

                    foundPlainProj = false;
                    if ((foundPlainProj = secondEntity->TryGetComponent<BasicProjectile>(projectilePtr)) || secondEntity->TryGetComponent<HomingProjectile>(homingProjectilePtr))
                    {
                        if (foundPlainProj)
                        {
                            projectilePtr->TryDamageEntity(firstEntity.get());
                        }
                        else
                        {
                            homingProjectilePtr->TryDamageEntity(firstEntity.get());
                        }
                    }

                    // TODO: stop collisions lol
                }

            }
        }
    }

    void Scene::OnDestroy()
    {
        for (int entityIndex = 0; entityIndex < entities.size(); entityIndex++)
        {
            entities[entityIndex]->OnDestroy();
        }
    }

    void InitLaunchScene(Scene* scene) {
        Ref<Entity> backgroundSection_Launch = scene->AddEntity("Launch Wall");// de toute boté `⎚⩊⎚´
        Ref<Renderable> renderablePtr = backgroundSection_Launch->AddComponent<Renderable>();
        renderablePtr->SetModeSprite(ResourceManager::Get("assets/UI-UX/lancement_du_jeu.png", ResourceType::Texture));
        backgroundSection_Launch->transform->Scale = Vector2(0.667f, 0.667f);

        Ref<LoadSceneOnInput> launchable = backgroundSection_Launch->AddComponent<LoadSceneOnInput>();
        launchable->Init("Home", sf::Keyboard::Key::Space);
    }

    void InitHomeScene(Scene* scene) {
        Ref<Entity> backgroundSection_Settings = scene->AddEntity("Settings Wall");// de toute boté `⎚⩊⎚´
        Ref<Renderable> renderablePtr = backgroundSection_Settings->AddComponent<Renderable>();
        renderablePtr->SetModeSprite(ResourceManager::Get("assets/UI-UX/assets_UX/fond.png", ResourceType::Texture));
        backgroundSection_Settings->transform->Scale = Vector2(0.667f, 0.667f);

        Ref<Entity> title_Settings = scene->AddEntity("Settings Title");
        renderablePtr = title_Settings->AddComponent<Renderable>();
        renderablePtr->SetModeSprite(ResourceManager::Get("assets/UI-UX/assets_UX/titre_mafia_shootout.png", ResourceType::Texture));
        title_Settings->transform->Scale = Vector2(0.667f, 0.667f);

        float x_Settings = (RenderSettings::ScreenWidth - renderablePtr->GetWidth()) / 2;

        title_Settings->transform->Position = Vector2(x_Settings, renderablePtr->GetHeight() / 2);

        float y_Settings = (float)RenderSettings::ScreenHeight / 2;

        Ref<Entity> game_button_2 = scene->AddEntity("Game Button");
        InitButton(game_button_2,
            Vector2(x_Settings, y_Settings),
            "assets/UI-UX/assets_UX/PLAY.png",
            [&]() { ScenesManager::LoadScene("Lvl1"); }
        );

        y_Settings += game_button_2->GetComponent<Renderable>()->GetHeight() + 20.0f;

        // Declaration des differents composant de l'ui pour les référencer dans settings_button
        Ref<Entity> difficulty_sprite = scene->AddEntity("Difficulty Sprite");
        Ref<Entity> difficulty_button_1 = scene->AddEntity("Difficulty Button 1");
        Ref<Entity> difficulty_button_2 = scene->AddEntity("Difficulty Button 2");
        Ref<Entity> difficulty_button_3 = scene->AddEntity("Difficulty Button 3");

        Ref<Entity> languages_sprite = scene->AddEntity("Languages Sprite");
        Ref<Entity> left_arrow = scene->AddEntity("Left Arrow");
        Ref<Entity> english_sprite = scene->AddEntity("English Sprite");
        Ref<Entity> right_arrow = scene->AddEntity("Left Arrow");

        Ref<Entity> volume_sprite = scene->AddEntity("Volume Sprite");
        Ref<Entity> volume_slider = scene->AddEntity("Volume Slider");

        Ref<Entity> settings_button2 = scene->AddEntity("Settings Button");
        InitButton(
            settings_button2,
            Vector2(x_Settings, y_Settings),
            "assets/UI-UX/assets_UX/SETTINGS.png",
            [=]() {
                if (difficulty_sprite->Enabled) {
                    difficulty_sprite->Disable();
                    difficulty_button_1->Disable();
                    difficulty_button_2->Disable();
                    difficulty_button_3->Disable();
                    languages_sprite->Disable();
                    left_arrow->Disable();
                    english_sprite->Disable();
                    right_arrow->Disable();
                    volume_sprite->Disable();
                    volume_slider->Disable();
                }
                else {
                    difficulty_sprite->Enable();
                    difficulty_button_1->Enable();
                    difficulty_button_2->Enable();
                    difficulty_button_3->Enable();
                    languages_sprite->Enable();
                    left_arrow->Enable();
                    english_sprite->Enable();
                    right_arrow->Enable();
                    volume_sprite->Enable();
                    volume_slider->Enable();
                }
            }
        );

        y_Settings += settings_button2->GetComponent<Renderable>()->GetHeight() + 20.0f;

        difficulty_sprite->transform->Scale = Vector2(0.667f, 0.667f);
        difficulty_sprite->transform->Position = Vector2(x_Settings, y_Settings);
        renderablePtr = difficulty_sprite->AddComponent<Renderable>();
        renderablePtr->SetModeSprite(ResourceManager::Get("assets/UI-UX/assets_UX/DIFFICULTY.png", ResourceType::Texture));

        Vector2 difficulty_x_y = Vector2(x_Settings + renderablePtr->GetWidth() * 1.75f, y_Settings);
        y_Settings += renderablePtr->GetHeight() + 20.0f;

        InitButton(
            difficulty_button_1,
            difficulty_x_y,
            "assets/UI-UX/assets_UX/niveau_difficulty.png",
            [difficulty_button_2, difficulty_button_3]() {
                std::cout << "Difficulty Button 1 Clicked" << std::endl;
                difficulty_button_2->GetComponent<Renderable>()->SetModeSprite(ResourceManager::Get("assets/UI-UX/assets_UX/niveau_difficulty_vide.png", ResourceType::Texture));
                difficulty_button_3->GetComponent<Renderable>()->SetModeSprite(ResourceManager::Get("assets/UI-UX/assets_UX/niveau_difficulty_vide.png", ResourceType::Texture));
            }
        );

        difficulty_x_y.x += difficulty_button_1->GetComponent<Renderable>()->GetWidth() + 10.0f;

        InitButton(
            difficulty_button_2,
            difficulty_x_y,
            "assets/UI-UX/assets_UX/niveau_difficulty.png",
            [difficulty_button_2, difficulty_button_3]() {
                std::cout << "Difficulty Button 2 Clicked" << std::endl;
                difficulty_button_2->GetComponent<Renderable>()->SetModeSprite(ResourceManager::Get("assets/UI-UX/assets_UX/niveau_difficulty.png", ResourceType::Texture));
                difficulty_button_3->GetComponent<Renderable>()->SetModeSprite(ResourceManager::Get("assets/UI-UX/assets_UX/niveau_difficulty_vide.png", ResourceType::Texture));
            }
        );

        difficulty_x_y.x += difficulty_button_2->GetComponent<Renderable>()->GetWidth() + 10.0f;

        InitButton(
            difficulty_button_3,
            difficulty_x_y,
            "assets/UI-UX/assets_UX/niveau_difficulty_vide.png",
            [difficulty_button_2, difficulty_button_3]() {
                std::cout << "Difficulty Button 3 Clicked" << std::endl;
                difficulty_button_2->GetComponent<Renderable>()->SetModeSprite(ResourceManager::Get("assets/UI-UX/assets_UX/niveau_difficulty.png", ResourceType::Texture));
                difficulty_button_3->GetComponent<Renderable>()->SetModeSprite(ResourceManager::Get("assets/UI-UX/assets_UX/niveau_difficulty.png", ResourceType::Texture));
            }
        );

        languages_sprite->transform->Scale = Vector2(0.667f, 0.667f);
        languages_sprite->transform->Position = Vector2(x_Settings, y_Settings);
        renderablePtr = languages_sprite->AddComponent<Renderable>();
        renderablePtr->SetModeSprite(ResourceManager::Get("assets/UI-UX/assets_UX/LANGUAGES.png", ResourceType::Texture));

        Vector2 language_x_y = Vector2(x_Settings + renderablePtr->GetWidth() * 1.5f, y_Settings);
        y_Settings += renderablePtr->GetHeight() + 20.0f;

        InitButton(
            left_arrow,
            language_x_y,
            "assets/UI-UX/assets_UX/fleche_gauche.png",
            []() { std::cout << "Left Arrow Clicked" << std::endl; }
        );

        language_x_y.x += left_arrow->GetComponent<Renderable>()->GetWidth() + 15.0f;

        english_sprite->transform->Scale = Vector2(0.667f, 0.667f);
        english_sprite->transform->Position = language_x_y;
        renderablePtr = english_sprite->AddComponent<Renderable>();
        renderablePtr->SetModeSprite(ResourceManager::Get("assets/UI-UX/assets_UX/ENGLISH.png", ResourceType::Texture));

        language_x_y.x += renderablePtr->GetWidth() + 15.0f;

        InitButton(
            right_arrow,
            language_x_y,
            "assets/UI-UX/assets_UX/fleche_droite.png",
            []() { std::cout << "Right Arrow Clicked" << std::endl; }
        );


        volume_sprite->transform->Scale = Vector2(0.667f, 0.667f);
        volume_sprite->transform->Position = Vector2(x_Settings, y_Settings);
        renderablePtr = volume_sprite->AddComponent<Renderable>();
        renderablePtr->SetModeSprite(ResourceManager::Get("assets/UI-UX/assets_UX/VOLUME.png", ResourceType::Texture));

        InitSlider(
            volume_slider,
            Vector2(0.667f, 0.667f), Vector2(x_Settings + renderablePtr->GetWidth() * 2, y_Settings),
            "assets/UI-UX/assets_UX/barre_son.png", "assets/UI-UX/assets_UX/bouton_son.png"
        );
        
        Ref<Slider> sliderPtr = volume_slider->GetComponent<Slider>();
        sliderPtr->SetMinMaxValues(0.0f, 100.0f);
        sliderPtr->SetCallback([](float volume) { AudioManager::GetAudioSource("MainMusic")->SetVolume(volume); });

        // immediately call to start with the settings closed
        settings_button2->GetComponent<OnClick>()->ManualCall();
    }

    void InitPauseScene(Scene* scene) {
        Ref<Renderable> renderablePtr;
        float border_height = 70.0f;

        Ref<Entity> background = scene->AddEntity("Background");
        renderablePtr = background->AddComponent<Renderable>();
        renderablePtr->SetModeSprite(ResourceManager::Get("assets/UI-UX/UI/fond_menu_in_game.png", ResourceType::Texture));
        background->transform->Scale = RenderSettings::ScreenSize / renderablePtr->GetSize();

        Vector2 ui_scale = background->transform->Scale;

        Ref<Entity> menu_sprite = scene->AddEntity("Menu Sprite");
        renderablePtr = menu_sprite->AddComponent<Renderable>();
        renderablePtr->SetModeSprite(ResourceManager::Get("assets/UI-UX/UI/MENU.png", ResourceType::Texture));
        menu_sprite->transform->Scale = ui_scale;
        menu_sprite->transform->Position = Vector2((RenderSettings::ScreenWidth - renderablePtr->GetWidth()) / 2, border_height);

        Vector2 menu_x_y = Vector2(RenderSettings::ScreenWidth / 8, RenderSettings::ScreenHeight / 4 + border_height);

        Ref<Entity> back_button = scene->AddEntity("Back Button");
        InitButton(
            back_button,
            menu_x_y,
            "assets/UI-UX/UI/BACK.png",
            []() { ScenesManager::LoadScene(ScenesManager::GetPreviousScene()); }
        );

        menu_x_y.y += back_button->GetComponent<Renderable>()->GetHeight() + 20.0f;

        // Declaration des differents composant de l'ui pour les référencer dans settings_button

        Ref<Entity> quit_button = scene->AddEntity("Quit");
        Ref<Entity> difficulty_sprite = scene->AddEntity("Difficulty Sprite");
        Ref<Entity> difficulty_button_1 = scene->AddEntity("Difficulty Button 1");
        Ref<Entity> difficulty_button_2 = scene->AddEntity("Difficulty Button 2");
        Ref<Entity> difficulty_button_3 = scene->AddEntity("Difficulty Button 3");

        Ref<Entity> languages_sprite = scene->AddEntity("Languages Sprite");
        Ref<Entity> left_arrow = scene->AddEntity("Left Arrow");
        Ref<Entity> english_sprite = scene->AddEntity("English Sprite");
        Ref<Entity> right_arrow = scene->AddEntity("Left Arrow");

        Ref<Entity> volume_sprite = scene->AddEntity("Volume Sprite");
        Ref<Entity> volume_slider = scene->AddEntity("Volume Slider");

        Ref<Entity> settings_button = scene->AddEntity("Settings Button");
        InitButton(
            settings_button,
            menu_x_y,
            "assets/UI-UX/assets_UX/SETTINGS.png",
            [=]() {
                if (quit_button->Enabled) {
                    quit_button->Disable();
                    difficulty_sprite->Disable();
                    difficulty_button_1->Disable();
                    difficulty_button_2->Disable();
                    difficulty_button_3->Disable();
                    languages_sprite->Disable();
                    left_arrow->Disable();
                    english_sprite->Disable();
                    right_arrow->Disable();
                    volume_sprite->Disable();
                    volume_slider->Disable();
                }
                else {
                    quit_button->Enable();
                    difficulty_sprite->Enable();
                    difficulty_button_1->Enable();
                    difficulty_button_2->Enable();
                    difficulty_button_3->Enable();
                    languages_sprite->Enable();
                    left_arrow->Enable();
                    english_sprite->Enable();
                    right_arrow->Enable();
                    volume_sprite->Enable();
                    volume_slider->Enable();
                }
            }
        );

        renderablePtr = quit_button->AddComponent<Renderable>();
        renderablePtr->SetModeSprite(ResourceManager::Get("assets/UI-UX/UI/QUIT.png", ResourceType::Texture));
        quit_button->transform->Scale = ui_scale;
        InitButton(
            quit_button,
            Vector2(
                (RenderSettings::ScreenWidth - renderablePtr->GetWidth()) / 2, 
                RenderSettings::ScreenHeight - (renderablePtr->GetHeight() / 2) - border_height
            ),
            "assets/UI-UX/UI/QUIT.png",
            []() { throw new std::runtime_error("Time to quit ^^"); } // He back :D
        );

        menu_x_y.y += settings_button->GetComponent<Renderable>()->GetHeight() + 20.0f;
        ui_scale = Vector2(0.667f, 0.667f);

        difficulty_sprite->transform->Scale = ui_scale;
        difficulty_sprite->transform->Position = menu_x_y;
        renderablePtr = difficulty_sprite->AddComponent<Renderable>();
        renderablePtr->SetModeSprite(ResourceManager::Get("assets/UI-UX/assets_UX/DIFFICULTY.png", ResourceType::Texture));

        Vector2 difficulty_x_y = Vector2(menu_x_y.x + renderablePtr->GetWidth() * 1.75f, menu_x_y.y);
        menu_x_y.y += renderablePtr->GetHeight() + 20.0f;

        InitButton(
            difficulty_button_1,
            difficulty_x_y,
            "assets/UI-UX/assets_UX/niveau_difficulty.png",
            [difficulty_button_2, difficulty_button_3]() {
                GameManager::SetDifficulty(0);
                difficulty_button_2->GetComponent<Renderable>()->SetModeSprite(ResourceManager::Get("assets/UI-UX/assets_UX/niveau_difficulty_vide.png", ResourceType::Texture));
                difficulty_button_3->GetComponent<Renderable>()->SetModeSprite(ResourceManager::Get("assets/UI-UX/assets_UX/niveau_difficulty_vide.png", ResourceType::Texture));
            }
        );

        difficulty_x_y.x += difficulty_button_1->GetComponent<Renderable>()->GetWidth() + 10.0f;

        InitButton(
            difficulty_button_2,
            difficulty_x_y,
            "assets/UI-UX/assets_UX/niveau_difficulty.png",
            [difficulty_button_2, difficulty_button_3]() {
                GameManager::SetDifficulty(1);
                difficulty_button_2->GetComponent<Renderable>()->SetModeSprite(ResourceManager::Get("assets/UI-UX/assets_UX/niveau_difficulty.png", ResourceType::Texture));
                difficulty_button_3->GetComponent<Renderable>()->SetModeSprite(ResourceManager::Get("assets/UI-UX/assets_UX/niveau_difficulty_vide.png", ResourceType::Texture));
            }
        );

        difficulty_x_y.x += difficulty_button_2->GetComponent<Renderable>()->GetWidth() + 10.0f;

        InitButton(
            difficulty_button_3,
            difficulty_x_y,
            "assets/UI-UX/assets_UX/niveau_difficulty_vide.png",
            [difficulty_button_2, difficulty_button_3]() {
                GameManager::SetDifficulty(2);
                difficulty_button_2->GetComponent<Renderable>()->SetModeSprite(ResourceManager::Get("assets/UI-UX/assets_UX/niveau_difficulty.png", ResourceType::Texture));
                difficulty_button_3->GetComponent<Renderable>()->SetModeSprite(ResourceManager::Get("assets/UI-UX/assets_UX/niveau_difficulty.png", ResourceType::Texture));
            }
        );

        languages_sprite->transform->Scale = ui_scale;
        languages_sprite->transform->Position = menu_x_y;
        renderablePtr = languages_sprite->AddComponent<Renderable>();
        renderablePtr->SetModeSprite(ResourceManager::Get("assets/UI-UX/assets_UX/LANGUAGES.png", ResourceType::Texture));

        Vector2 language_x_y = Vector2(menu_x_y.x + renderablePtr->GetWidth() * 1.5f, menu_x_y.y);
        menu_x_y.y += renderablePtr->GetHeight() + 20.0f;

        InitButton(
            left_arrow,
            language_x_y,
            "assets/UI-UX/assets_UX/fleche_gauche.png",
            []() { std::cout << "Left Arrow Clicked" << std::endl; }
        );

        language_x_y.x += left_arrow->GetComponent<Renderable>()->GetWidth() + 15.0f;

        english_sprite->transform->Scale = ui_scale;
        english_sprite->transform->Position = language_x_y;
        renderablePtr = english_sprite->AddComponent<Renderable>();
        renderablePtr->SetModeSprite(ResourceManager::Get("assets/UI-UX/assets_UX/ENGLISH.png", ResourceType::Texture));

        language_x_y.x += renderablePtr->GetWidth() + 15.0f;

        InitButton(
            right_arrow,
            language_x_y,
            "assets/UI-UX/assets_UX/fleche_droite.png",
            []() { std::cout << "Right Arrow Clicked" << std::endl; }
        );

        volume_sprite->transform->Scale = ui_scale;
        volume_sprite->transform->Position = menu_x_y;
        renderablePtr = volume_sprite->AddComponent<Renderable>();
        renderablePtr->SetModeSprite(ResourceManager::Get("assets/UI-UX/assets_UX/VOLUME.png", ResourceType::Texture));

        InitSlider(
            volume_slider,
            ui_scale,
            Vector2(menu_x_y.x + renderablePtr->GetWidth() * 2, menu_x_y.y),
            "assets/UI-UX/assets_UX/barre_son.png", "assets/UI-UX/assets_UX/bouton_son.png"
        );
        Ref<Slider> sliderPtr = volume_slider->GetComponent<Slider>();
        sliderPtr->SetMinMaxValues(0.0f, 100.0f);
        sliderPtr->SetCallback([](float newVolume) { AudioManager::GetAudioSource("MainMusic")->SetVolume(newVolume); });

        // immediately call to start with the settings closed
        settings_button->GetComponent<OnClick>()->ManualCall();
    }

    void InitLvl1Scene(Scene* scene) {
        Ref<Renderable> renderablePtr;

#pragma region Environment

        float environment_scale;

#pragma region Background Initialization

        for (int x = 0; x < 2; x++)
        {
            Ref<Entity> backgroundSection = scene->AddEntity("Background" + std::to_string(x));
            renderablePtr = backgroundSection->AddComponent<Renderable>();
            renderablePtr->SetModeSprite(ResourceManager::Get("assets/sprites/Route.png", ResourceType::Texture));

            Vector2 background_scale = { RenderSettings::ScreenWidth / renderablePtr->GetWidth(), RenderSettings::ScreenHeight / renderablePtr->GetHeight() };
            environment_scale = background_scale.y;
            backgroundSection->transform->Scale = background_scale;
            backgroundSection->transform->Position = Vector2(
                renderablePtr->GetWidth() * x, // (effective width of the background) * x-> so one section at X = 0 and one waiting for its turn just at the end of the previous section
                .0f
            );

            { auto _ = backgroundSection->AddComponent<Scrollable>(); }
        }

        scene->AddOnAwake([]() { GameManager::bounds = Vector2((float)RenderSettings::ScreenHeight / 5, 3 * (float)RenderSettings::ScreenHeight / 4); });

#pragma endregion
        
        std::vector<std::string> building_paths = {
            "assets/sprites/yellow_up.png", "assets/sprites/yellow_down.png",
            "assets/sprites/brown_up.png", "assets/sprites/brown_down.png",
            "assets/sprites/Hopital.png", "assets/sprites/Tribunal.png"
        };

        float sidewalk_size = (float)RenderSettings::ScreenHeight / 5;
        float building_offset = 0;

        for (u32 i = 0; i < building_paths.size(); i++) {
            Ref<Entity> building = scene->AddEntity("BuildingUp" + std::to_string(i));
            renderablePtr = building->AddComponent<Renderable>();
            renderablePtr->SetModeSprite(ResourceManager::Get(building_paths[i], ResourceType::Texture));
    
            building->transform->Scale = { environment_scale, environment_scale };
            building->transform->Position = { building_offset, sidewalk_size - renderablePtr->GetHeight() };

            building_offset += renderablePtr->GetWidth() + 20.0f;

            { auto _ = building->AddComponent<Scrollable>(); }
        }

        sidewalk_size = (float)RenderSettings::ScreenHeight / 4;
        building_offset = 0;

        for (i32 i = building_paths.size() - 1; i >= 0; i--) {
            Ref<Entity> building = scene->AddEntity("BuildingDown" + std::to_string(i));
            renderablePtr = building->AddComponent<Renderable>();
            renderablePtr->SetModeSprite(ResourceManager::Get(building_paths[i], ResourceType::Texture));

            float scale_factor = sidewalk_size / renderablePtr->GetHeight();

            building->transform->Scale = { environment_scale, environment_scale };
            building->transform->Position = { building_offset, RenderSettings::ScreenHeight - sidewalk_size };

            building_offset += renderablePtr->GetWidth() + 20.0f;

            { auto _ = building->AddComponent<Scrollable>(); }
        }

        int secondHeight = 800;
        for (int height = 0; height < secondHeight + 1; height += secondHeight)
        {
            for (int i = 0; i < 5; i++)
            {
                Ref<Entity> cloud = scene->AddEntity("Cloud" + std::to_string(i));
                renderablePtr = cloud->AddComponent<Renderable>();
                renderablePtr->SetModeSprite(ResourceManager::Get("assets/sprites/clouds.png", ResourceType::Texture));


                cloud->transform->Scale = Vector2::Random(30, 45) * .01f;
                cloud->transform->Position = Vector2(
                    rand() % (RenderSettings::ScreenWidth * 2) - renderablePtr->GetWidth(),
                    rand() % 300 - renderablePtr->GetHeight() + height
                );

                Ref<Scrollable> scrollablePtr = cloud->AddComponent<Scrollable>();
                scrollablePtr->ScrollingSpeed = rand() % 500 + 250;
            }
        }

        Ref<Entity> fond_ATH_sprite = scene->AddEntity("Fond ATH Sprite");
        renderablePtr = fond_ATH_sprite->AddComponent<Renderable>();
        renderablePtr->SetModeSprite(ResourceManager::Get("assets/UI-UX/UI/fond_ATH.png", ResourceType::Texture));
        fond_ATH_sprite->transform->Scale = Vector2(0.54f, 0.36f);
        fond_ATH_sprite->transform->Position = Vector2::Zero;

        for (int i = 0; i < 5; i++) {
            Ref<Entity> life_sprite = scene->AddEntity("Life Sprite " + std::to_string(i));
            renderablePtr = life_sprite->AddComponent<Renderable>();
            renderablePtr->SetModeSprite(ResourceManager::Get("assets/UI-UX/UI/rose_vie.png", ResourceType::Texture));
            life_sprite->transform->Scale = Vector2(.2f, .2f);
            life_sprite->transform->Position = Vector2(38.84f * i, .0f);
        }

        Ref<Entity> separation_ATH_sprite = scene->AddEntity("Separation ATH Sprite");
        renderablePtr = separation_ATH_sprite->AddComponent<Renderable>();
        renderablePtr->SetModeSprite(ResourceManager::Get("assets/UI-UX/UI/separation_ATH.png", ResourceType::Texture));
        separation_ATH_sprite->transform->Scale = Vector2(.05f, .05f);
        separation_ATH_sprite->transform->Position = Vector2(206.5f, .0f);

        Ref<Entity> scores_sprite = scene->AddEntity("Scores Sprite");
        renderablePtr = scores_sprite->AddComponent<Renderable>();
        renderablePtr->SetModeSprite(ResourceManager::Get("assets/UI-UX/UI/SCORE.png", ResourceType::Texture));
        scores_sprite->transform->Scale = Vector2(1.0f, 1.0f);
        scores_sprite->transform->Position = Vector2(222.84f, 7.0f);

#pragma endregion

        Ref<Entity> player = scene->AddEntity("Player");
        InitPlayer(player);
        player->GetComponent<PlayerMovement>()->bounds = Vector2((float)RenderSettings::ScreenHeight / 5, 3 * (float)RenderSettings::ScreenHeight / 4);
    }

    void InitLvl2Scene(Scene* scene) {
        Ref<Renderable> renderablePtr;

        for (int i = 0; i < 2; i++) {
            Ref<Entity> background = scene->AddEntity("Background" + std::to_string(i));
            renderablePtr = background->AddComponent<Renderable>();
            renderablePtr->SetModeSprite(ResourceManager::Get("assets/Fx/sea.png", ResourceType::Texture));
            background->transform->Scale = RenderSettings::ScreenSize / renderablePtr->GetSize();
            background->transform->Position = Vector2(renderablePtr->GetWidth() * i, .0f);

            Ref<Scrollable> background_scrollable = background->AddComponent<Scrollable>();
            background_scrollable->ScrollingSpeed = 200;
        }

        for (int i = 0; i < 2; i++) {
            Ref<Entity> road = scene->AddEntity("Road" + std::to_string(i));
            renderablePtr = road->AddComponent<Renderable>();
            renderablePtr->SetModeSprite(ResourceManager::Get("assets/Batiments/Route_pont.png", ResourceType::Texture));
            road->transform->Scale = Vector2(RenderSettings::ScreenWidth / renderablePtr->GetWidth(), RenderSettings::ScreenHeight * 2 / 3 / renderablePtr->GetHeight());
            road->transform->Position = Vector2(renderablePtr->GetWidth() * i, (RenderSettings::ScreenHeight - renderablePtr->GetHeight()) / 2);

            { auto _ = road->AddComponent<Scrollable>(); }
        }

        Vector2 road_bounds = Vector2(160.0f, 532.0f);
        scene->AddOnAwake([road_bounds]() { GameManager::bounds = road_bounds; });

        Ref<Entity> player = scene->AddEntity("Player");
        InitPlayer(player);
        player->GetComponent<PlayerMovement>()->bounds = road_bounds;

        std::array<std::pair<std::string, std::string>, 2> bridge_pieces = {
            {
                {"Middle", "assets/Batiments/Pont_milieu.png"},
                {"Right", "assets/Batiments/Pont_droite.png"}
            }
        };
        
        for (int i = 0; i < 2; i++) {
            Ref<Entity> bridge = scene->AddEntity("Bridge " + bridge_pieces[i].first);
            renderablePtr = bridge->AddComponent<Renderable>();
            renderablePtr->SetModeSprite(ResourceManager::Get(bridge_pieces[i].second, ResourceType::Texture));
            bridge->transform->Scale = RenderSettings::ScreenSize / renderablePtr->GetSize();
            bridge->transform->Position = Vector2(renderablePtr->GetWidth() * i, (RenderSettings::ScreenHeight - renderablePtr->GetHeight()) / 2);

            { auto _ = bridge->AddComponent<Scrollable>(); }
        }
        
        int secondHeight = 800;
        for (int height = 0; height < secondHeight + 1; height += secondHeight)
        {
            for (int i = 0; i < 5; i++)
            {
                Ref<Entity> cloud = scene->AddEntity("Cloud" + std::to_string(i));
                renderablePtr = cloud->AddComponent<Renderable>();
                renderablePtr->SetModeSprite(ResourceManager::Get("assets/sprites/clouds.png", ResourceType::Texture));


                cloud->transform->Scale = Vector2::Random(30, 45) * .01f;
                cloud->transform->Position = Vector2(
                    rand() % (RenderSettings::ScreenWidth * 2) - renderablePtr->GetWidth(),
                    rand() % 300 - renderablePtr->GetHeight() + height
                );

                Ref<Scrollable> scrollablePtr = cloud->AddComponent<Scrollable>();
                scrollablePtr->ScrollingSpeed = rand() % 500 + 250;
            }
        }

        #pragma region UI

        Ref<Entity> fond_ATH_sprite = scene->AddEntity("Fond ATH Sprite");
        renderablePtr = fond_ATH_sprite->AddComponent<Renderable>();
        renderablePtr->SetModeSprite(ResourceManager::Get("assets/UI-UX/UI/fond_ATH.png", ResourceType::Texture));
        fond_ATH_sprite->transform->Scale = Vector2(0.54f, 0.36f);
        fond_ATH_sprite->transform->Position = Vector2::Zero;

        for (int i = 0; i < 5; i++) {
            Ref<Entity> life_sprite = scene->AddEntity("Life Sprite " + std::to_string(i));
            renderablePtr = life_sprite->AddComponent<Renderable>();
            renderablePtr->SetModeSprite(ResourceManager::Get("assets/UI-UX/UI/rose_vie.png", ResourceType::Texture));
            life_sprite->transform->Scale = Vector2(.2f, .2f);
            life_sprite->transform->Position = Vector2(38.84f * i, .0f);
        }

        Ref<Entity> separation_ATH_sprite = scene->AddEntity("Separation ATH Sprite");
        renderablePtr = separation_ATH_sprite->AddComponent<Renderable>();
        renderablePtr->SetModeSprite(ResourceManager::Get("assets/UI-UX/UI/separation_ATH.png", ResourceType::Texture));
        separation_ATH_sprite->transform->Scale = Vector2(.05f, .05f);
        separation_ATH_sprite->transform->Position = Vector2(206.5f, .0f);

        Ref<Entity> scores_sprite = scene->AddEntity("Scores Sprite");
        renderablePtr = scores_sprite->AddComponent<Renderable>();
        renderablePtr->SetModeSprite(ResourceManager::Get("assets/UI-UX/UI/SCORE.png", ResourceType::Texture));
        scores_sprite->transform->Scale = Vector2(1.0f, 1.0f);
        scores_sprite->transform->Position = Vector2(222.84f, 7.0f);

        // Ref<Entity> line = scene->AddEntity("Line");
        // renderablePtr = line->AddComponent<Renderable>();
        // renderablePtr->SetModeRect(Vector2(RenderSettings::ScreenWidth, 1.0f), sf::Color::White);
        
        // { auto _ = line->AddComponent<Mover>(); };

        #pragma endregion

    }

    void InitWinScene(Scene* scene) {
        Ref<Renderable> renderablePtr;

        Ref<Entity> background = scene->AddEntity("Background");
        renderablePtr = background->AddComponent<Renderable>();
        renderablePtr->SetModeSprite(ResourceManager::Get("assets/UI-UX/UI/background.png", ResourceType::Texture));
        background->transform->Scale = RenderSettings::ScreenSize / renderablePtr->GetSize();

        Vector2 ui_scale = background->transform->Scale;

        Ref<Entity> victory_sprite = scene->AddEntity("Victory Sprite");
        renderablePtr = victory_sprite->AddComponent<Renderable>();
        renderablePtr->SetModeSprite(ResourceManager::Get("assets/UI-UX/UI/VICTORY.png", ResourceType::Texture));
        victory_sprite->transform->Scale = ui_scale;
        victory_sprite->transform->Position = Vector2(
            (RenderSettings::ScreenWidth - renderablePtr->GetWidth()) / 2,
            70.0f
        );

        Ref<Entity> flowers_sprite = scene->AddEntity("Flowers Sprite");
        renderablePtr = flowers_sprite->AddComponent<Renderable>();
        renderablePtr->SetModeSprite(ResourceManager::Get("assets/UI-UX/UI/roses.png", ResourceType::Texture));
        flowers_sprite->transform->Scale = ui_scale;
        flowers_sprite->transform->Position = Vector2(
            (RenderSettings::ScreenWidth - renderablePtr->GetWidth()) / 2,
            victory_sprite->transform->Position.y + 40.0f
        );

        Vector2 ui_x_y = Vector2(victory_sprite->transform->Position.x - 70.0f, RenderSettings::ScreenHeight / 2 + 70.0f);

        Ref<Entity> score_sprite = scene->AddEntity("Score Sprite");
        renderablePtr = score_sprite->AddComponent<Renderable>();
        renderablePtr->SetModeSprite(ResourceManager::Get("assets/UI-UX/UI/SCORE.png", ResourceType::Texture));
        score_sprite->transform->Scale = Vector2(2.05f, 2.05f);
        score_sprite->transform->Position = ui_x_y;

        ui_x_y.y += renderablePtr->GetHeight() * 2 + 20.0f;

        Ref<Entity> restart_button = scene->AddEntity("Restart Button");
        InitButton(
            restart_button,
            ui_x_y,
            "assets/UI-UX/UI/RETRY.png",
            []() {
                const std::string previous_scene = ScenesManager::GetPreviousScene();
                ScenesManager::RemoveScene(previous_scene);
                Scene* lvl_scene = ScenesManager::AddScene(previous_scene);
                previous_scene == "Lvl1" ? InitLvl1Scene(lvl_scene) : InitLvl2Scene(lvl_scene);
                ScenesManager::LoadScene(previous_scene);
            }
        );
        restart_button->transform->Scale = Vector2(0.5f, 0.5f);

        ui_x_y.x += restart_button->GetComponent<Renderable>()->GetWidth() * 1.5f;

        Ref<Entity> continue_button = scene->AddEntity("Continue Button");
        InitButton(
            continue_button,
            ui_x_y,
            "assets/UI-UX/UI/CONTINUE.png",
            []() { ScenesManager::LoadScene("Lvl2"); } // PlaceHolder function :D
        );
        continue_button->transform->Scale = Vector2(0.77f, 0.77f);

        Ref<Entity> scoreCounter = scene->AddEntity("ScoreCounter");
        Ref<Renderable> scoreCounterRenderablePtr = scoreCounter->AddComponent<Renderable>();
        scoreCounterRenderablePtr->SetModeText(std::to_string(GameManager::GetScore()), ResourceManager::Get("assets/fonts/Roboto-Black.ttf", ResourceType::Font), 25, sf::Color::White);
        scoreCounterRenderablePtr->SetOrigin(true);
        scoreCounter->AddComponent<Mover>();
        scoreCounter->AddComponent<RescalerUniform>();
        scoreCounter->transform->Position = Vector2(800, 435);
        scoreCounter->transform->Scale = Vector2(3, 3);
    }

    void InitGameOverScene(Scene* scene) {
        Ref<Renderable> renderablePtr;

        Ref<Entity> background = scene->AddEntity("Background");
        renderablePtr = background->AddComponent<Renderable>();
        renderablePtr->SetModeSprite(ResourceManager::Get("assets/UI-UX/UI/background.png", ResourceType::Texture));
        background->transform->Scale = RenderSettings::ScreenSize / renderablePtr->GetSize();

        Ref<Entity> game_over_sprite = scene->AddEntity("Game Over Sprite");
        renderablePtr = game_over_sprite->AddComponent<Renderable>();
        renderablePtr->SetModeSprite(ResourceManager::Get("assets/UI-UX/UI/GAME_OVER.png", ResourceType::Texture));
        game_over_sprite->transform->Scale = background->transform->Scale;
        game_over_sprite->transform->Position = Vector2(
            (RenderSettings::ScreenWidth - renderablePtr->GetWidth()) / 2,
            100.0f
        );

        Ref<Entity> line_1 = scene->AddEntity("Line 1");
        renderablePtr = line_1->AddComponent<Renderable>();
        line_1->transform->Position = Vector2(.0f, 235.3f);
        renderablePtr->SetModeRect(
            Vector2(RenderSettings::ScreenWidth, 1.0f),
            sf::Color::White
        );

        Ref<Entity> score_sprite = scene->AddEntity("Score Sprite");
        renderablePtr = score_sprite->AddComponent<Renderable>();
        renderablePtr->SetModeSprite(ResourceManager::Get("assets/UI-UX/UI/SCORE.png", ResourceType::Texture));
        score_sprite->transform->Scale = Vector2(2.05f, 2.05f);
        score_sprite->transform->Position = Vector2(171.37f, 289.26f);

        Ref<Entity> line_2 = scene->AddEntity("Line 2");
        renderablePtr = line_2->AddComponent<Renderable>();
        line_2->transform->Position = Vector2(.0f, 397.25f);
        renderablePtr->SetModeRect(
            Vector2(RenderSettings::ScreenWidth, 1.0f),
            sf::Color::White
        );

        Ref<Entity> quit_button = scene->AddEntity("Quit Button");
        InitButton(
            quit_button,
            Vector2(229.41f, 476.56f),
            "assets/UI-UX/UI/QUIT.png",
            []() { throw new std::runtime_error("Time to quit ^^"); }
        );
        quit_button->transform->Scale = Vector2(1.14f, 1.14f);

        Ref<Entity> retry_button = scene->AddEntity("Retry Button");
        InitButton(
            retry_button,
            Vector2(791.8f, 472.85f),
            "assets/UI-UX/UI/RETRY.png",
            []() {
                const std::string previous_scene = ScenesManager::GetPreviousScene();
                ScenesManager::RemoveScene(previous_scene);
                Scene* lvl_scene = ScenesManager::AddScene(previous_scene);
                previous_scene == "Lvl1" ? InitLvl1Scene(lvl_scene) : InitLvl2Scene(lvl_scene);
                ScenesManager::LoadScene(previous_scene);
            }
        );

        Ref<Entity> line_3 = scene->AddEntity("Line 3");
        renderablePtr = line_3->AddComponent<Renderable>();
        line_3->transform->Position = Vector2(RenderSettings::ScreenWidth / 2, 397.25f);
        renderablePtr->SetModeRect(
            Vector2(1.0f, 182.5f),
            sf::Color::White
        );

        Ref<Entity> ring_sprite = scene->AddEntity("Ring Sprite");
        renderablePtr = ring_sprite->AddComponent<Renderable>();
        renderablePtr->SetModeSprite(ResourceManager::Get("assets/UI-UX/UI/ring_blood.png", ResourceType::Texture));
        ring_sprite->transform->Scale = Vector2(0.82f, 0.82f);
        ring_sprite->transform->Position = Vector2((RenderSettings::ScreenWidth - renderablePtr->GetWidth()) / 2, 590.0f);

        Ref<Entity> scoreCounter = scene->AddEntity("ScoreCounter");
        Ref<Renderable> scoreCounterRenderablePtr = scoreCounter->AddComponent<Renderable>();

        scoreCounterRenderablePtr->SetModeText(std::to_string(GameManager::GetScore()), ResourceManager::Get("assets/fonts/Roboto-Black.ttf", ResourceType::Font), 25, sf::Color::White);
        scoreCounterRenderablePtr->SetOrigin(true);
        scoreCounter->transform->Position = Vector2(925, 300);
        scoreCounter->transform->Scale = Vector2(3, 3);
    }

}