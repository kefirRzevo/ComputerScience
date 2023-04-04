#pragma once


#include "math.hpp"
#include "texture.hpp"


//----------------------------------------//

class Window
{
    private:

        sf::RenderWindow sfRenderWindow;

    public:

        Window(const Vec2u& size, const char* name):
            sfRenderWindow(sf::VideoMode(size.x, size.y), name) {}

        sf::RenderWindow*
        Get_SF_RenderWindow();
};

//----------------------------------------//

class Renderer
{
    private:

        sf::RenderWindow* sfRenderWindow;

        sf::Color sfColor;

        float thickness;

        Renderer(Window* window);

    public:

        static Renderer*
        Get(Window* sfRenderWindow_ = nullptr);

        sf::RenderWindow*
        Get_SF_RenderWindow();

        void
        SetColor(Color color);
        void
        SetThickness(float thickness_);

        void
        DrawPixel(const Vec2i& pos);
        void
        DrawCircle(const Vec2i& center, int radius, size_t nPoints = 100);
        void
        DrawThickLine(const Vec2i& point, float degreeAngle, float length);
        void
        DrawThickLineSlow(const Vec2i& p1, const Vec2i& p2);
        void
        DrawLine(const Vec2i& p1, const Vec2i& p2);
        void
        DrawRect(const RectInt& RectInt);

        void
        DrawTexture(const Texture& src, const RectInt& dstRectInt);
        void
        DrawText(const Text& text_);

        bool
        OnRender() const;

        void
        Clear();
        void
        Display();
        void
        Close();
};

//----------------------------------------//
