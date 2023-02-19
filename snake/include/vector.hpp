#pragma once

#include <iostream>

template <typename T>
class Vector2
{
    private:
        T x;
        T y;
    
    public:
        Vector2(T X, T Y):
            x(X), y(Y) {};

        Vector2(const Vector2<T>& v):
            x(v.x), y(v.y) {};

        Vector2():
            x(0), y(0) {};

        ~Vector2()
            {};

        T GetX() const
            {
                return x;
            }

        T GetY() const
            {
                return y;
            }        

        Vector2& operator=(const Vector2<T>& v)
            {
                return Vector2(v);
            }

        Vector2& operator-()
            {
                x*=-1;
                y*=-1;
                return *this;
            }

        Vector2& operator+=(const Vector2<T>& v)
            {
                x+=v.x;
                y+=v.y;
                return *this;
            }

        Vector2& operator-=(const Vector2<T>& v)
            {
                x-=v.x;
                y-=v.y;
                return *this;
            }

        Vector2& operator*=(const T& k)
            {
                x*=k;
                y*=k;
                return *this;
            }

        Vector2& operator/=(const T& k)
            {
                x/=k;
                y/=k;
                return *this;
            }

        friend Vector2 operator+(const Vector2& lv, const Vector2& rv)
            {
                return Vector2(lv.x+rv.x, lv.y+rv.y);
            }

        friend Vector2 operator-(const Vector2& lv, const Vector2& rv)
            {
                return Vector2(lv.x-rv.x, lv.y-rv.y);
            }

        friend Vector2 operator*(const T& lv, const Vector2& rv)
            {
                return Vector2(lv*rv.x, lv*rv.y);
            }

        friend Vector2 operator*(const Vector2& lv, const T& rv)
            {
                return Vector2(lv.x*rv, rv*lv.y);
            }
};

using iVec = Vector2<int>;
using uVec = Vector2<unsigned int>;
