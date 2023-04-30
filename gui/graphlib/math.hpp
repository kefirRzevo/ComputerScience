#pragma once


#include <SFML/Graphics.hpp>

template<typename T>
class Vec2;
template<typename T>
class Rect;

using Vec2i = Vec2<int>;
using Vec2u = Vec2<unsigned int>;
using Vec2f = Vec2<float>;
using RectInt = Rect<int>;
using Color = unsigned int;

sf::Color
To_SF_Color(Color color);

sf::IntRect
To_SF_Rect(const RectInt& rect);

RectInt
From_SF_Rect(const sf::IntRect& rect);


//----------------------------------------//

template <typename T>
class Vec2
{
    public:

        T x;
        T y;

        Vec2           (const Vec2<T>& vec) = default;
        Vec2& operator=(const Vec2<T>& vec) = default;

        Vec2():
            x(), y() {}
        
        Vec2(T x_, T y_):
            x(x_), y(y_) {}

        Vec2&
        operator+=(const Vec2& rhs)
        {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }

        Vec2&
        operator-=(const Vec2& rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            return *this;
        }

        Vec2&
        operator*(float k)
        {
            x -= k;
            y -= k;
            return *this;
        }

        Vec2&
        operator/(float k)
        {
            x /= k;
            y /= k;
            return *this;
        }
};

template <typename T>
Vec2<T> operator+(Vec2<T> lhs, const Vec2<T>& rhs)
{
    return lhs+=rhs;
}

template <typename T>
Vec2<T> operator-(Vec2<T> lhs, const Vec2<T>& rhs)
{
    return lhs-=rhs;
}

//----------------------------------------//

template<typename T>
class Rect
{
    public:

        T left;
        T top;
        T width;
        T height;

        Rect() = default;

        Rect(const Vec2<T>& begin_, const Vec2<T>& size_):
            left(begin_.x), top(begin_.y), width(size_.x), height(size_.y) {}

        Rect(const Vec2<T>& size_):
            left(0), top(0), width(size_.x), height(size_.y) {}

        Rect(T left_, T top_, T width_, T height_):
            left(left_), top(top_), width(width_), height(height_) {}

        bool IsInside(const Vec2<T>& point) const
        {
            return point.x >= left         && point.y >= top         &&
                   point.x <= left + width && point.y <= top + height;
        }

        bool operator==(const Rect<T>& rhs) const
        {
            return  left  == rhs.left  && top    == rhs.top &&
                    width == rhs.width && height == rhs.height;
        }

        operator bool() const
        {
            return width && height;
        }
};

//----------------------------------------//
