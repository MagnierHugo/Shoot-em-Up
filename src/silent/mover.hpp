// mover.hpp

#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"

#include "Component.hpp"
#include "Transform.hpp"
#include "vector.hpp"
#include "renderer.hpp"


namespace sl {

    class Mover : public Component {
    public:
        void Awake() override
        {
            UpdateWindow();
            Vector2 renderable_size = Owner->GetComponent<Renderable>()->GetSize();
            xWindowSize = Vector2(.0f, RenderSettings::ScreenWidth - renderable_size.x);
            yWindowSize = Vector2(.0f, RenderSettings::ScreenHeight - renderable_size.y);

        }

        void Update(const float deltaTime) override
        {
            ImGui::Begin("Move me baby");

            // ImGui::InputFloat("Window Size", &windowSize);
            // if (ImGui::Button("Reset Slider Window", (Vector2::One * 25 + Vector2::Right * 200))) { UpdateWindow(); }

            ImGui::SliderFloat((Owner->name + ": X").c_str(), &Transform()->Position.x, xWindowSize.x, xWindowSize.y, "%fpx");
            ImGui::SameLine();
            ImGui::InputFloat((Owner->name + ": Set X").c_str(), &Transform()->Position.x);

            if (Transform()->Position.x < xWindowSize.x || Transform()->Position.x > xWindowSize.y) { UpdateWindow(); }

            ImGui::SliderFloat((Owner->name + ": Y").c_str(), &Transform()->Position.y, yWindowSize.x, yWindowSize.y, "%fpx");
            ImGui::SameLine();
            ImGui::InputFloat((Owner->name + ": Set Y").c_str(), &Transform()->Position.y);

            if (Transform()->Position.y < yWindowSize.x || Transform()->Position.y > yWindowSize.y) { UpdateWindow(); }

            ImGui::End();
        }

    private:
        float windowSize;
        Vector2 xWindowSize = Vector2::Zero;
        Vector2 yWindowSize = Vector2::Zero;
        void UpdateWindow()
        {
            xWindowSize = Vector2(
                Transform()->Position.x - windowSize,
                Transform()->Position.x + windowSize
            );

            yWindowSize = Vector2(
                Transform()->Position.y - windowSize,
                Transform()->Position.y + windowSize
            );
        }
    };

}