// button.hpp

#pragma once

#include <functional>

#include "Component.hpp"
#include "Renderable.hpp"

namespace sl {

    class OnClick : public Component {
    private:
        void Clicked();

    public:
        void Awake() override;
        void Update(const float deltaTime) override;

        void SetCallback(std::function<void(void)> callback);
        void ManualCall() const;

    private:
        Ref<Renderable> renderable;
        std::function<void(void)> on_click;
        b8 clickable = true;

    };

    void InitButton(Ref<Entity> button, Vector2 topleft, std::string path, std::function<void(void)> callback);

}