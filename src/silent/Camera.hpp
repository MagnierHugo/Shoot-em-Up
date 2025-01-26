#include <vector>

#include "Component.hpp"
#include "screen_shake.hpp"
#include "screen_shake_duration.hpp"

//#define TEST_SHAKE

namespace sl
{
    class Camera : public Component
    {
    public:
        sf::Vector2f ViewLookAt;
        sf::Vector2f ViewSize;
        float ZoomFactor = 1.0f;

        Camera();
        ~Camera();

        void Awake() override;
        void Update(const float) override;


        void static ApplyScreenShake(const sl::Vector2& intensity, float durationInMS);
        void static ApplyScreenShake(const float xIntensity, const float yIntensity, float durationInMS);
        void static ApplyScreenShake(const float xIntensity, const float yIntensity, ScreenShakeDuration duration);
        void static ResetScreenShake();


        void static Zoom(const float factor);
        void static SetZoom(const float newZoomFactor);
        void static ResetZoom();
    
    private:
        std::vector<ScreenShake*> screenShakeValues;
    #ifdef TEST_SHAKE
        int shakeIntensityX = 25; // please for god s sake don t exceed 50 (50 is stupid already)
        int shakeIntensityY = 25; // please for god s sake don t exceed 50 (50 is stupid already )
        int shakeDuration = 100;
    #endif
        static Camera* instance;
    };
}
