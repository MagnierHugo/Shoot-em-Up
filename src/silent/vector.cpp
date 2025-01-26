// vector.cpp

#define _USE_MATH_DEFINES
#include <cmath>

#include "vector.hpp"
#include "SFML/System/Vector2.hpp"

namespace sl {

    const Vector2 Vector2::Zero = Vector2(.0f, .0f);
    const Vector2 Vector2::One = Vector2(1.0f, 1.0f);
    const Vector2 Vector2::Left = Vector2(-1.0f, .0f);
    const Vector2 Vector2::Right = Vector2(1.0f, .0f);
    const Vector2 Vector2::Up = Vector2(.0f, -1.0f);
    const Vector2 Vector2::Down = Vector2(.0f, 1.0f);
    const Vector2 Vector2::InvertX = Vector2(-1.0f, 1.0f);
    const Vector2 Vector2::InvertY = Vector2(1.0f, -1.0f);
    
    float Vector2::Distance(const Vector2& other) const
    {
        int xOffset = other.x - x;
        int yOffset = other.y - y;
        return sqrt(xOffset * xOffset + yOffset * yOffset);
    }

    float Vector2::Length() const
    {
        return std::sqrt(x * x + y * y);
    }

    float Vector2::SquaredLength() const {
        return x * x + y * y;
    }

    Vector2 Vector2::Normalized() const
    {
        float length = Length();

        return Vector2(x / length, y / length);
    }

    Vector2 Vector2::NormalizedIfNonZero() const
    {
        return x == .0f && y == .0f ? Zero : Normalized();
    }

    void Vector2::Normalize()
    {
        float length = Length();

        x /= length;
        y /= length;
    }

    float Vector2::Dot(const Vector2& other) const{
        return x * other.x + y * other.y;
    }
    
	void Vector2::Rotate(float angle) {
        float radians = angle * M_PI / 180;
        x = x * std::cos(radians) - y * std::sin(radians);
        y = x * std::sin(radians) + y * std::cos(radians);
    }
    
	Vector2 Vector2::Rotated(float angle) const {
        float radians = angle * M_PI / 180;
        return Vector2(x * std::cos(radians) - y * std::sin(radians), x * std::sin(radians) + y * std::cos(radians));
    }

    Vector2 Vector2::Random(int min, int max)
    {
        return Vector2(
            (rand() % max - min + 1) + min,
            (rand() % max - min + 1) + min
        );
    }

    Vector2 Vector2::Copy() const
    {
        return Vector2(x, y);
    }

    bool Vector2::operator==(const Vector2& other) const
    {
        return (x == other.x) && (y == other.y);
    }

    bool Vector2::operator>(const Vector2& other) const {
        return x + y > other.x + other.y;
    }

    bool Vector2::operator<(const Vector2& other) const {
        return x + y < other.x + other.y;
    }

    bool Vector2::operator>=(const Vector2& other) const {
        return x + y >= other.x + other.y;
    }

    bool Vector2::operator<=(const Vector2& other) const {
        return x + y <= other.x + other.y;
    }

    Vector2& Vector2::operator=(const Vector2& other)
    {
        x = other.x;
        y = other.y;
        return *this;
    }

    bool Vector2::operator!=(const Vector2& other)
    {
        return x != other.x || y != other.y;
    }

    Vector2 Vector2::operator+(const Vector2& other) const
    {
        return Vector2(x + other.x, y + other.y);
    }

    Vector2 Vector2::operator-(const Vector2& other) const
    {
        return Vector2(x - other.x, y - other.y);
    }

    Vector2 Vector2::operator*(const Vector2& other) const
    {
        return Vector2(x * other.x, y * other.y);
    }
    
	Vector2 Vector2::operator/(const Vector2& other) const {
        return Vector2(x / other.x, y / other.y);
    }

    Vector2 Vector2::operator*(const float scalar) const
    {
        return Vector2(x * scalar, y * scalar);
    }

    Vector2 Vector2::operator/(const float scalar) const
    {
        return Vector2(x / scalar, y / scalar);
    }

    Vector2& Vector2::operator+=(const Vector2& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector2& Vector2::operator-=(const Vector2& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Vector2& Vector2::operator*=(const float scalar)
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    Vector2& Vector2::operator/=(const float scalar)
    {
        x /= scalar;
        y /= scalar;
        return *this;
    }
    Vector2::operator sf::Vector2f()
    {
        return sf::Vector2f(x, y);
    }

    Vector2::operator sf::Vector2u()
    {
        return sf::Vector2u(x, y);
    }

    Vector2::operator sf::Vector2i()
    {
        return sf::Vector2i(x, y);
    }

    Vector2::operator ImVec2()
    {
        return ImVec2(x, y);
    }

}