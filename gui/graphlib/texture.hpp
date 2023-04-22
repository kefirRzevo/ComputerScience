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
    public:

        Font(const char* path);

    private:

        sf::Font sfFont;

    friend class Text;
};

//----------------------------------------//

class FontManager
{
    private:

        std::unordered_map<const char*, const Font&> fonts;
        const Font& defaultFont;

        FontManager();

    public:

        ~FontManager();

        static FontManager*
        Get();

        const Font&
        GetFont(const char* path_);

        const Font&
        GetDefaultFont() const;
    
    friend class Renderer;
};

//----------------------------------------//

class Text
{
    public:

        Text(const std::string& string_ = "",
        const Font& font = FontManager::Get()->GetDefaultFont(),
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
    public:

        Texture(Vec2i size_);
        Texture(Color color_);
        Texture(const char* path);
        Texture(const char* path, int i, int j, int w, int h);
        Texture(const char* path, const RectInt& textureRect);
        ~Texture();

        std::unique_ptr<Color>
        ToBuffer() const;

        void
        FromBuffer(Color* pixels);
        void
        FromBuffer(Color* pixels, int w, int h, int x, int y);

    private:

        bool isManagerOwner;

        const sf::Texture* sfSrcTexture;
        sf::IntRect  sfTextureRect;
        sf::Sprite   sfSprite;

    friend class Renderer;
};

//----------------------------------------//

class TextureManager
{
    private:

        std::unordered_map<const char*, const sf::Texture*> sfSrcTextures;
        const sf::Texture* defaultSfSrcTexture;

    public:

        TextureManager();

        ~TextureManager();

        static TextureManager*
        Get();

        const sf::Texture*
        GetTexture(const char* path);

        const sf::Texture*
        GetDefaultTexture() const;

    friend class Renderer;
};

//----------------------------------------//
