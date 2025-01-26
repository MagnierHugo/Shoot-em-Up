// renderer.hpp

#pragma once

#include <vector>

#include "scenes_manager.hpp"

namespace sl
{
    struct RenderSettings
    {
    public:
        RenderSettings();        

        sf::RenderTarget* target;

        b8 use_shaders;
        b8 color_grade;
        b8 bloom;
        b8 cast_shadows;

        static sl::Vector2 ScreenSize;
        static int ScreenWidth;
        static int ScreenHeight;
        static sf::Texture TakeScreenshot();
    private:
        static RenderSettings* instance;
    }; 

    class Renderer
    {
    private:

        struct RenderEntityArgs {
            RenderEntityArgs(Ref<Entity> entity_, RenderSettings* settings_, Vector2 sunPos)
            {
                entity = entity_;
                settings = settings_;
                sun_pos = sunPos;
            }
            Ref<Entity> entity;
            RenderSettings* settings;
            Vector2 sun_pos;
        };

    public:
        Renderer();
        ~Renderer() = default;

        void RenderScenes(RenderSettings& settings, ScenesManager* scenes);
        void RenderEntity(RenderEntityArgs& args);
    private:
        sf::Shader sprite_shader;
    };
}
