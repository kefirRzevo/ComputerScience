#pragma once


#include <SFML/Graphics.hpp>
#include <iostream>
#include <cassert>
#include <queue>
#include <unordered_map>

#include "../include/config.hpp"

template<typename T>
class Vec2;
class Rect;

class Font;
class Text;

class Event;
class Texture;

class EventManager;
class TextureManager;

class Window;
class Renderer;

using Vec2i = Vec2<int>;
using Vec2u = Vec2<unsigned int>;

//----------------------------------------//

using Color = unsigned int;

const Color White = 0xFF'FF'FF'FF;
const Color Black = 0x00'00'00'00;
const Color Red   = 0xFF'FF'00'00;
const Color Green = 0xFF'00'FF'00;
const Color Blue  = 0xFF'00'00'FF;

sf::Color
To_SF_Color(Color color);

//----------------------------------------//

template <typename T>
class Vec2
{
    public:

        T x;
        T y;

        Vec2():
            x(), y() {}
        
        Vec2(T x_, T y_):
            x(x_), y(y_) {}

        Vec2&
        operator+(const Vec2& rhs)
        {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }

        Vec2&
        operator-(const Vec2& rhs)
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
    return lhs += rhs;
}

template <typename T>
Vec2<T> operator-(Vec2<T> lhs, const Vec2<T>& rhs)
{
    return lhs -= rhs;
}

//----------------------------------------//

class Rect
{
    public:

        Vec2i begin;
        Vec2i size;

        Rect()
            {}

        Rect(const Vec2i& begin_, const Vec2i& size_):
            begin(begin_), size(size_) {}

        bool IsInside(const Vec2i& point) const;
};

//----------------------------------------//

class Event
{
    public:

        struct KeyValue
        {
            int key;

            KeyValue(int key_):
                key(key_) {}
        };

        struct MouseValue
        {
            Vec2i click;
            Vec2i offset;

            MouseValue(Vec2i click_):
                click(click_) {}
            
            MouseValue(Vec2i click_, Vec2i offset_):
                click(click_), offset(offset_) {}
        };

        enum EventType
        {
            quit,
            keyPressed,
            mousePressed,
            mouseReleased,
            mouseHovered,
            mouseMoved,

            count
        };

        union EventValue
        {
            MouseValue mouse;
            KeyValue key;

            EventValue()
                {}

            EventValue(int key_):
                key(key_) {}

            EventValue(Vec2i click_):
                mouse(click_) {}

            EventValue(Vec2i click_, Vec2i offset_):
                mouse(click_, offset_) {}
        };

        EventType type;
        EventValue value;

        Event()
            {}

        Event(EventType type_):
            type(type_) {}

        Event(EventType type_, const EventValue& value_):
            type(type_), value(value_) {}

        bool Poll();
};

//----------------------------------------//

class EventManager
{
    private:

        std::queue<Event> events;

        EventManager() = default;

        bool Poll_SF_Event();

    public:

        static EventManager*
        Get();

        void
        PushEvent(const Event& event);

        bool
        PollEvent(Event& event);
};

//----------------------------------------//

class Window //: public sf::RenderTarget
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

/*
class RenderTexture: public sf::RenderTarget
{
    private:

        sf::RenderTexture sfRenderTexture;

    public:

        RenderTexture(const Vec2u& size);

        sf::RenderTexture*
        Get_SF_RenderTexture();
};
*/

//----------------------------------------//

class Font
{
    private:

        sf::Font sfFont;

    public:

        Font(const char* path);

        const sf::Font*
        Get_SF_Font() const;
};

//----------------------------------------//

class Text
{
    private:

        sf::Text sfText;

    public:

        Text(const Font& font, const char* text, Color color, size_t size);

        sf::Text*
        Get_SF_Text();
};

//----------------------------------------//

class Texture
{
    private:

        sf::Texture sfTexture;

    public:

        Texture(const Vec2u& size);
        Texture(const char* path);
        Texture(const char* path, const Vec2u& size);
        Texture(const char* path, const Rect& textureRect);

        sf::Texture*
        Get_SF_Texture();

        Vec2u
        GetSize() const;

        const Color*
        ToBuffer() const;

        void
        FromBuffer(Color* pixels);
};

//----------------------------------------//

class TextureManager
{
    private:

        std::unordered_map<std::string, Texture* > textures;

        Texture* defaultTexture;

        TextureManager();

    public:

        ~TextureManager();

        static TextureManager*
        Get();

        Texture*
        GetTexture(const std::string& path);

        Texture*
        GetDefaultTexture();
};

//----------------------------------------//

class Renderer
{
    private:

        sf::RenderWindow* sfRenderWindow;

        sf::Color sfColor;

        Renderer(Window* window);

    public:

        static Renderer*
        Get(Window* sfRenderWindow_ = nullptr);

        sf::RenderWindow*
        Get_SF_RenderWindow();

        void
        SetColor(Color color);

        void
        DrawPixel(const Vec2i& pos);
        void
        DrawCircle(const Vec2i& center, int radius);
        void
        DrawLine(const Vec2i& p1, const Vec2i& p2);
        void
        DrawLine(const Vec2i& p1, const Vec2i& p2, size_t thickness);
        void
        DrawRect(const Rect& rect);
        void
        DrawRect(const Rect& rect, size_t thickness);

        void
        DrawTexture(Texture* src, const Rect& rscRect);
        void
        DrawTexture(Texture* src, const Rect& rscRect, const Rect& dstRect);
        void
        DrawText(const Vec2i& pos, const Text& text);

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
