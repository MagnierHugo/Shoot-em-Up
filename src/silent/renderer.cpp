// renderer.cpp

#include <cassert>
#include <iostream>

#include "renderer.hpp"
#include "Camera.hpp"
#include "Renderable.hpp"
#include "LightSource.hpp"

namespace sl
{
    int RenderSettings::ScreenWidth = 1280;
    int RenderSettings::ScreenHeight = 720;
    Vector2 RenderSettings::ScreenSize = Vector2(RenderSettings::ScreenWidth, RenderSettings::ScreenHeight = 720);
    RenderSettings* RenderSettings::instance;

    sl::RenderSettings::RenderSettings()
    {
        instance = this;

        target = nullptr;

        use_shaders = false;
        color_grade = false;
        bloom = false;
        cast_shadows = false;
    }

    sf::Texture sl::RenderSettings::TakeScreenshot()
    {
        sf::Texture texture;
        //sf::Image img = ((sf::RenderWindow*)instance->target)->capture();
        //img.saveToFile("screenshot.png");
        auto _ = texture.loadFromImage(((sf::RenderWindow*)instance->target)->capture());

        return texture;
    }

    Renderer::Renderer()
    {
        if (!sf::Shader::isAvailable()) {
            std::cout << "Somehow you don't have shaders. Wow." << std::endl;
            assert(false);
        }

        sprite_shader.loadFromFile("assets/shaders/sprite_v.glsl", "assets/shaders/sprite_f.glsl");
    }

    void Renderer::RenderScenes(RenderSettings& settings, ScenesManager* scenes)
    {
        sl::Ref<Camera> cameraPtr = scenes->persistentScene->GetEntity("Camera")->GetComponent<Camera>();
        sf::View view;

        view.setSize(cameraPtr->ViewSize);
        view.setCenter(cameraPtr->ViewLookAt);
        view.zoom(cameraPtr->ZoomFactor);

        settings.target->setView(view);


        std::vector<Ref<LightSource>> lights; // kinda nasty but don t wanna loop through each scene twice
        for (auto& [_, scene] : scenes->scenes)
        {
            if (!scene->Active) { continue; }


            for (auto& lightSource : scene->lightSources)
            {
                if (!lightSource->Enabled) { continue; }

                lights.push_back(lightSource);
            }
        }

        // set shader values (light pos)

        RenderEntityArgs renderArgs = RenderEntityArgs(nullptr, &settings, sl::Vector2::Zero);
        for (auto& [_, scene] : scenes->scenes)
        {
            if (!scene->Active) { continue; }


            for (auto& sceneEntity : scene->entities)
            {
                if (!sceneEntity->Enabled) { continue; }

                renderArgs.entity = sceneEntity;
                RenderEntity(renderArgs);

                for (auto& childEntity : sceneEntity->childEntities)
                {
                    if (!childEntity->Enabled) { continue; }

                    renderArgs.entity = childEntity;
                    RenderEntity(renderArgs);
                }
            }
        }
    }

    void Renderer::RenderEntity(RenderEntityArgs& args)
    {
        sl::Ref<Renderable> renderablePtr = nullptr;
        if (!args.entity->TryGetComponent<Renderable>(renderablePtr)) { return; }
        if (!renderablePtr->ShouldRender) { return; }

        sprite_shader.setUniform("uUsingTexture", renderablePtr->ShapeType == ShapeType::Sprite);
        //sprite_shader.setUniform("uLightPos", (sf::Glsl::Vec2)sun_pos);

#ifdef WANNA_DEBUG_SOME_POINT
        sf::CircleShape debugDot;
        debugDot.setScale(1.0f, 1.0f);
        debugDot.setFillColor(sf::Color::Red);
        debugDot.setRadius(3);
#endif
        
        sf::Drawable* drawable = renderablePtr->GetDrawable();
        if (drawable == nullptr) { return; }

        if (args.settings->use_shaders) { args.settings->target->draw(*drawable, &sprite_shader); }
        else { args.settings->target->draw(*drawable); }

#ifdef WANNA_DEBUG_SOME_POINT
        debugDot.setPosition(renderablePtr->Text.getPosition());
        args.settings->target->draw(debugDot);
#endif
    }
}
