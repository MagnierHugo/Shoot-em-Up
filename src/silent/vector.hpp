// vector.hpp

#pragma once

#include <iostream>

#include "SFML/Graphics.hpp"
#include "SFML/window.hpp"
#include "SFML/system.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"


namespace sl {
	
	class Vector2
	{
	public:
		float x;
		float y;
		
		const static Vector2 Zero;
		const static Vector2 One;
		const static Vector2 Left;
		const static Vector2 Right;
		const static Vector2 Up;
		const static Vector2 Down;
		const static Vector2 InvertX;
		const static Vector2 InvertY;

		Vector2() : x(.0f), y(.0f) {}
		Vector2(float _x, float _y) : x(_x), y(_y) {}
		Vector2(const Vector2& point) : x(point.x), y(point.y) {}
		Vector2(const sf::Vector2f& vector) : x(vector.x), y(vector.y) {}
		Vector2(const sf::Vector2u& vector) : x(vector.x), y(vector.y) {}
		Vector2(const sf::Vector2i& vector) : x(vector.x), y(vector.y) {}

		float Distance(const Vector2& other) const;
		float Length() const;
		float SquaredLength() const;
		Vector2 Normalized() const;
		Vector2 NormalizedIfNonZero() const;
		void Normalize();
		float Dot(const Vector2& other) const;
		Vector2 Copy() const;
		void Rotate(float angle); // angle in degrees
		Vector2 Rotated(float angle) const; //angle in degrees
		static Vector2 Random(int min, int max);

		bool operator==(const Vector2& other) const;
		bool operator>(const Vector2& other) const;
		bool operator<(const Vector2& other) const;
		bool operator>=(const Vector2& other) const;
		bool operator<=(const Vector2& other) const;
		Vector2& operator=(const Vector2& other);

		bool operator!=(const Vector2& other);
		Vector2 operator+(const Vector2& other) const;
		Vector2 operator-(const Vector2& other) const;
		Vector2 operator*(const Vector2& other) const;
		Vector2 operator/(const Vector2& other) const;
		Vector2 operator*(const float scalar) const;
		Vector2 operator/(const float scalar) const;
		Vector2& operator+=(const Vector2& other);
		Vector2& operator-=(const Vector2& other);
		Vector2& operator*=(const float scalar);
		Vector2& operator/=(const float scalar);

		operator sf::Vector2f();
		operator sf::Vector2u();
		operator sf::Vector2i();
		operator ImVec2();
		
		friend std::ostream& operator<<(std::ostream& os, const Vector2& point)
		{
			os << "(" << point.x << ", " << point.y << ")";
			return os;
		}
	};
}