#pragma once


#include "math.hpp"
#include "texture.hpp"

class EventManager;
class Renderer;

//----------------------------------------//

class Window
{
    private:

        sf::RenderWindow sfRenderWindow;

    public:

        Window(Vec2u size_, const std::string& name_);

    friend class Renderer;
};

//----------------------------------------//

class Renderer
{
    private:

        static std::unique_ptr<Renderer> renderer;

        sf::RenderWindow* sfRenderWindow;

        sf::Color sfColor;
        float thickness;

        Renderer(Window* window);

    public:

        static void
        Initialize(Window* window_);

        static Renderer*
        Get();

        void
        SetColor(Color color_);
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
        DrawTexture(Texture& src, const RectInt& dstRectInt);
        void
        DrawText(Text& text_);

        bool
        OnRender() const;

        void
        Clear();
        void
        Display();
        void
        Close();
    
    friend class EventManager;
};

//----------------------------------------//
