#pragma once


#include "math.hpp"
#include "config.hpp"

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <iostream>


//----------------------------------------//

class Font
{
    private:

        sf::Font sfFont;

    public:

        Font(const char* path);

        const sf::Font&
        Get_SF_Font() const;
};

//----------------------------------------//

class FontManager
{
    private:

        std::unordered_map<const char*, Font* > fonts;

        const Font* defaultFont;

        FontManager();

    public:

        ~FontManager();

        static FontManager*
        Get();

        const Font&
        GetFont(const char* path);

        const Font&
        GetDefaultFont() const;
};

//----------------------------------------//

class Text
{
    private:

        sf::Text sfText;

    public:

        Text(const Font& font, const char* text, Color color, size_t size);

        Vec2i
        GetSize() const;

        Vec2i
        GetPosition() const;

        void
        SetPosition(Vec2i pos_);

        void
        SetString(const char* string_);

        void
        Move(Vec2i delta_);

        const sf::Text&
        Get_SF_Text() const;
};

//----------------------------------------//

class Texture
{
    private:

        bool isManagerOwner;
        const sf::Texture* sfSrcTexture;
        sf::IntRect sfTextureRect;

    public:

        Texture(const Vec2i& size);
        Texture(const char* path);
        Texture(const char* path, int i, int j, int w, int h);
        Texture(const char* path, const RectInt& textureRect);
        ~Texture();

        const sf::Texture&
        Get_SF_Texture() const;

        const sf::IntRect&
        Get_SF_TextureRect() const;

        std::unique_ptr<Color>
        ToBuffer() const;

        void
        FromBuffer(Color* pixels);
        void
        FromBuffer(Color* pixels, int w, int h, int x, int y);
};

//----------------------------------------//

class TextureManager
{
    private:

        std::unordered_map<std::string, sf::Texture* > sfSrcTextures;

        const sf::Texture* defaultSfSrcTexture;

        TextureManager();

    public:

        ~TextureManager();

        static TextureManager*
        Get();

        const sf::Texture&
        GetTexture(const char* path);

        const sf::Texture&
        GetDefaultTexture() const;
};

//----------------------------------------//
