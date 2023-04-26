#pragma once


#include "math.hpp"
#include "../config.hpp"

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <iostream>

class Font;
class FontManager;
class Text;
class Texture;
class TextureManager;

class Renderer;

//----------------------------------------//

class Font
{
    private:

        Font(const char* path);

        sf::Font sfFont;

    friend class FontManager;
    friend class Text;
};

//----------------------------------------//

class FontManager
{
    private:

        static std::unique_ptr<FontManager> fontManager;
        std::unordered_map<const char*, Font*> fonts;
        Font* defaultFont;

        FontManager();

    public:

        ~FontManager();

        static FontManager*
        Get();

        Font*
        GetFont(const char* path_);

        Font*
        GetDefaultFont();
};

//----------------------------------------//

class Text
{
    public:

        Text(const std::string& string_ = "",
        Font* font  = FontManager::Get()->GetDefaultFont(),
        Color color = Config::defTextColor, int size = Config::defTextSize);

        Vec2i
        GetSize() const;
        Vec2i
        GetPosition() const;
        std::string
        GetString() const;

        void
        Move(Vec2i delta_);
        void
        SetPosition(Vec2i pos_);
        void
        SetString(const std::string& string_);

    private:

        sf::Text sfText;

    friend class Renderer;
};

//----------------------------------------//

class Texture
{
    private:

        Texture(const char* path);
        Texture(const char* path, int i, int j, int w, int h);
        Texture(const char* path, const RectInt& textureRect);

    public:

        Texture           (              ) = delete;
        Texture           (const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;
        Texture           (Texture&&) = default;
        Texture& operator=(Texture&&) = default;
        ~Texture() = default;

        Texture(Vec2i size_);
        Texture(Color color_);

        int
        GetWidth() const;
        int
        GetHeight() const;
        bool
        ManagerOwners() const;

        Color*
        ToBuffer() const;
        void
        FromBuffer(Color* pixels);
        void
        FromBuffer(Color* pixels, int w, int h, int x, int y);

    private:

        bool managerOwners;

        sf::Texture sfSrcTexture;
        sf::IntRect sfSrcRect;
        Color       color;

    friend class TextureManager;
    friend class Renderer;
};

//----------------------------------------//

class TextureManager
{
    private:

        static std::unique_ptr<TextureManager> textureManager;
        std::unordered_map<std::string, Texture*> textures;
        Texture* defaultTexture;

    public:

        TextureManager();

        ~TextureManager();

        static TextureManager*
        Get();

        Texture*
        GetTexture(const char* path_);
        Texture*
        GetTexture(const char* path_, const RectInt& rect_);
        Texture*
        GetTexture(const char* path_, int i, int j, int w, int h);

        Texture*
        GetDefaultTexture();
};

//----------------------------------------//
