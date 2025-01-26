// main.cpp


#include <ctime>  

#include <SFML/Graphics.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui-SFML.h>

#include "silent/common.hpp"
#include "silent/scene.hpp"
#include "silent/scenes_manager.hpp"
#include "silent/renderer.hpp"
#include "silent/resource_manager.hpp"
#include "silent/input_manager.hpp"
#include "silent/Camera.hpp"
#include "silent/Renderable.hpp"
#include "silent/time.hpp"
#include "silent/scrollable.hpp"
#include "silent/player.hpp"
#include "silent/tilemap.hpp"
#include "silent/multicast_delegate.hpp"
#include "silent/button.hpp"
#include "silent/slider.hpp"
#include "silent/somewhat_smart_enemy.hpp"
#include "silent/sine_wave_enemy.hpp"
#include "silent/transform.hpp"
#include "silent/enemies.hpp"
#include "silent/idamageable.hpp"
#include "silent/boss.hpp"
#include "silent/load_on_input.hpp"
#include "silent/audio_manager.hpp"

//#define _DEBUG

int main(void)
{
    //srand((unsigned int)std::time(nullptr)); // used for clouds but was lowkey prettier on average XD 

    sf::RenderWindow window(sf::VideoMode(sl::RenderSettings::ScreenWidth, sl::RenderSettings::ScreenHeight), "Hello", sf::Style::Default);
    window.setFramerateLimit(144);

    sl::AudioManager audioManager;

    // Lines commented because main music need to be removed in order to make the repo public
    // for copyright reasons
    // sl::Ref<sl::AudioResource> audioResource = sl::AudioManager::CreateAudioResource("assets/music/music.mp3", sl::AudioType::Music);
    // sl::AudioManager::CreateAudioSource("MainMusic")->SetAudioResource(sl::AudioManager::GetAudioResource("assets/music/music.mp3"))->Play(true);

    sl::InputManager input(&window);

    sl::ScenesManager scenes;
    
    sl::RenderSettings settings;
    settings.target = &window;
    settings.use_shaders = false;

    sl::Scene* persistentScene = sl::ScenesManager::GetPersistentScene();
    
    sl::Ref<sl::Entity> camera = persistentScene->AddEntity("Camera");
    { auto _ = camera->AddComponent<sl::Camera>(); }

// GOOD JOB TRISTAN `⎚⩊⎚´ 
    sl::Scene* launchScene = sl::ScenesManager::AddScene("Launch");
    sl::InitLaunchScene(launchScene);
    sl::Scene* homeScene = sl::ScenesManager::AddScene("Home"); // PTN mais Tristan est cho sa mer `⎚⩊⎚´ //update (en fait non)
    sl::InitHomeScene(homeScene);
    sl::Scene* lvl1_scene = sl::ScenesManager::AddScene("Lvl1");
    sl::InitLvl1Scene(lvl1_scene);
    sl::Scene* lvl2_scene = sl::ScenesManager::AddScene("Lvl2");
    sl::InitLvl2Scene(lvl2_scene);
    sl::Scene* pauseScene = sl::ScenesManager::AddScene("Pause");
    sl::InitPauseScene(pauseScene);
    sl::Scene* gameOverScene = sl::ScenesManager::AddScene("GameOver");
    sl::InitGameOverScene(gameOverScene);
    sl::Scene* winScene = sl::ScenesManager::AddScene("Win");
    sl::InitWinScene(winScene);

    sl::ScenesManager::LoadScene("Launch");

    sl::Renderer renderer;

    sl::Ref<sl::Renderable> renderablePtr = nullptr;


    ImGui::SFML::Init(window);
    
    while (window.isOpen()) {
        sf::Event e;
        input.ResetInputState();
        while (window.pollEvent(e)) {
            input.Update(e);
            ImGui::SFML::ProcessEvent(e);
            if (e.type == sf::Event::Closed) {
                window.close();
            }
        }

        sl::Time::Tick();

        ImGui::SFML::Update(window, sl::Time::GetRawDeltaTime());
        scenes.UpdateRelevantScenes(sl::Time::GetDeltaTime());

#ifdef _DEBUG
        ImGui::Begin("Renderer Settings");
        ImGui::Checkbox("Use Shaders", reinterpret_cast<bool*>(&settings.use_shaders));
        ImGui::End();
#endif
        window.clear(sf::Color::Black);
        renderer.RenderScenes(settings, &scenes);
        ImGui::SFML::Render(window);
        window.display();
    }

    scenes.OnDestroy();

    ImGui::SFML::Shutdown();

    sl::ResourceManager::ClearResources();

    return 0;
}
