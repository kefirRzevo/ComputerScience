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
        sf::RenderTarget* sfRenderTarget;

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
        DrawPixel(Vec2i pos);
        void
        DrawPixel(Texture* src, Vec2i pos);
        void
        DrawCircle(Vec2i center, int radius);
        void
        DrawCircle(Texture* src, Vec2i center, int radius);

        void
        DrawThickLine(Vec2i point, float degreeAngle, float length, int thickness);
        void
        DrawThickLine(Texture* src, Vec2i point, float degreeAngle, float length, int thickness);
        void
        DrawThickLineSlow(Vec2i p1, Vec2i p2, int thickness);
        void
        DrawThickLineSlow(Texture* src, Vec2i p1, Vec2i p2, int thickness);
        void
        DrawLine(Vec2i p1, Vec2i p2);
        void
        DrawLine(Texture* src, Vec2i p1, Vec2i p2);

        void
        DrawRect(const RectInt& RectInt);
        void
        DrawRect(Texture* src, const RectInt& rect);
        void
        DrawTexture(const Texture* dst, const RectInt& dstRectInt);
        void
        DrawTexture(Texture* src, const Texture* dst, const RectInt& dstRect);
        void
        DrawText(const Text* text);
        void
        DrawText(Texture* src, const Text* text);
        void
        Clear();
        void
        Clear(Texture* src);

        void
        Close();
        void
        Display();
        bool
        OnRender() const;
    
    friend class EventManager;
};

//----------------------------------------//
