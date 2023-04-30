#include <sstream>

#include "texture.hpp"
#include "../config.hpp"

std::unique_ptr<FontManager>    FontManager::fontManager;
std::unique_ptr<TextureManager> TextureManager::textureManager;


//----------------------------------------//

std::string
SF_RectToString(const sf::IntRect& rect)
{
    std::ostringstream string;
    string << rect.left << " " << rect.top << " " << rect.width << " " << rect.height;
    return string.str();
}

//----------------------------------------//

std::string
ColorToString(Color color)
{
    std::ostringstream string;
    string << color;
    return string.str();
}

//----------------------------------------//

Font::Font(const char* path)
{
    sfFont.loadFromFile(path);
}

//----------------------------------------//

FontManager::FontManager():
defaultFont(GetFont(Config::defTextFontPath))
{}

FontManager::~FontManager()
{
    for(auto i: fonts)
        delete i.second;
}

FontManager*
FontManager::Get()
{
    if(fontManager.get() == nullptr)
        fontManager.reset(new FontManager{});

    return fontManager.get();
}

Font*
FontManager::GetFont(const char* path)
{
    auto fontIt = fonts.find(path);

    if(fontIt != fonts.end())
        return fontIt->second;

    Font* newFont = new Font{path};
    fonts.insert({path, newFont});

    return newFont;
}

Font*
FontManager::GetDefaultFont()
{
    return defaultFont;
}

//----------------------------------------//

Text::Text(const std::string& string_,
Font* font_, Color color_, int size_)
{
    sfText.setString(string_);
    sfText.setFont(font_->sfFont);
    sfText.setFillColor(To_SF_Color(color_));
    sfText.setCharacterSize(size_);
}

Vec2i
Text::GetSize() const
{
    return {static_cast<int>(sfText.getLocalBounds().width),
            static_cast<int>(sfText.getLocalBounds().height)};
}

Vec2i
Text::GetPosition() const
{
    return {static_cast<int>(sfText.getPosition().x),
            static_cast<int>(sfText.getPosition().y)};
}

std::string
Text::GetString() const
{
    return sfText.getString();
}

void
Text::SetString(const std::string& string_)
{
    sfText.setString(string_);
}

void
Text::SetPosition(Vec2i pos_)
{
    sfText.setPosition(static_cast<float>(pos_.x), static_cast<float>(pos_.y));
}

void
Text::Move(Vec2i delta_)
{
    sfText.move(static_cast<float>(delta_.x), static_cast<float>(delta_.y));
}

//----------------------------------------//

Texture::Texture(const char* path_):
managerOwners(false), color(0)
{
    if(!sfSrcTexture.loadFromFile(path_))
        assert(0);

    sfSrcTexture.loadFromFile(path_);
    sfSrcRect = {{}, static_cast<sf::Vector2i>(sfSrcTexture.getSize())};
}

Texture::Texture(const char* path_, int i, int j, int w, int h):
managerOwners(false), color(0)
{
    if(!sfSrcTexture.loadFromFile(path_))
        assert(0);

    int width  = static_cast<int>(sfSrcTexture.getSize().x);
    int height = static_cast<int>(sfSrcTexture.getSize().y);
    sfSrcRect = {width * i / w, height * j / h, width / h, height / h};

    sfSrcTexture.loadFromFile(path_, sfSrcRect);
}

Texture::Texture(const char* path_, const RectInt& rect):
managerOwners(false), color(0)
{
    sfSrcRect = {rect.left, rect.top, rect.width, rect.height};
    sfSrcTexture.loadFromFile(path_, sfSrcRect);
}

Texture::Texture(Vec2i size_):
managerOwners(false), sfSrcRect(0, 0, size_.x, size_.y), color(0)
{
    sfSrcTexture.create(static_cast<unsigned int>(size_.x),
                        static_cast<unsigned int>(size_.y));
}

Texture::Texture(Color color_):
managerOwners(false), sfSrcRect(0, 0, Config::defWidth, Config::defHeight),
color(color_)
{
    sfSrcTexture.create(static_cast<unsigned int>(sfSrcRect.width),
                        static_cast<unsigned int>(sfSrcRect.height));
}

int
Texture::GetWidth() const
{
    return sfSrcRect.width;
}

int
Texture::GetHeight() const
{
    return sfSrcRect.height;
}

bool
Texture::ManagerOwners() const
{
    return managerOwners;
}

Color*
Texture::ToBuffer() const
{
    sf::Image sfImage = sfSrcTexture.copyToImage();
    unsigned int numberPixels = sfImage.getSize().x * sfImage.getSize().y;

    const Color* oldPixels = reinterpret_cast<const Color*>(sfImage.getPixelsPtr());

    Color* newPixels = new Color[numberPixels];
    std::memcpy(newPixels, oldPixels, numberPixels);
    return newPixels;
}

void
Texture::FromBuffer(Color* pixels)
{
    sfSrcTexture.update(reinterpret_cast<sf::Uint8*>(pixels));
}

void
Texture::FromBuffer(Color* pixels, int w, int h, int x, int y)
{
    sfSrcTexture.update(reinterpret_cast<sf::Uint8*>(pixels), w, h, x, y);
}

//----------------------------------------//

TextureManager::TextureManager():
defaultTexture(GetTexture(Config::defColor))
{}

TextureManager::~TextureManager()
{
    for(auto i: textures)
        delete i.second;
}

TextureManager*
TextureManager::Get()
{
    if(textureManager.get() == nullptr)
        textureManager.reset(new TextureManager{});

    return textureManager.get();
}

Texture*
TextureManager::GetTexture(const char* path_)
{
    sf::Texture temp{};
    if(!temp.loadFromFile(path_))
        return nullptr;

    sf::IntRect sfRect = sf::IntRect{{}, static_cast<sf::Vector2i>(temp.getSize())};
    std::string key = path_ + SF_RectToString(sfRect);

    auto textureIt = textures.find(key);
    if(textureIt != textures.end())
        return textureIt->second;

    Texture* newTexture = new Texture{path_};
    newTexture->managerOwners = true;
    textures.insert({key, newTexture});

    return newTexture;
}

Texture*
TextureManager::GetTexture(const char* path_, const RectInt& rect_)
{
    if(!rect_.operator bool())
        return GetTexture(path_);

    sf::Texture temp{};
    if(!temp.loadFromFile(path_))
        return nullptr;

    sf::IntRect sfRect = sf::IntRect{rect_.left, rect_.top, rect_.width, rect_.height};
    std::string key = path_ + SF_RectToString(sfRect);

    auto textureIt = textures.find(key);
    if(textureIt != textures.end())
        return textureIt->second;

    Texture* newTexture = new Texture{path_, rect_};
    newTexture->managerOwners = true;
    textures.insert({key, newTexture});

    return newTexture;
}

Texture*
TextureManager::GetTexture(const char* path_, int i, int j, int w, int h)
{
    if(!(w && h))
        return GetTexture(path_);

    sf::Texture temp{};
    if(!temp.loadFromFile(path_))
        return nullptr;

    int width  = static_cast<int>(temp.getSize().x);
    int height = static_cast<int>(temp.getSize().y);
    sf::IntRect sfRect = sf::IntRect{width * i / w, height * j / h, width / h, height / h};
    std::string key = path_ + SF_RectToString(sfRect);

    auto textureIt = textures.find(key);
    if(textureIt != textures.end())
        return textureIt->second;

    Texture* newTexture = new Texture{path_, i, j, w, h};
    newTexture->managerOwners = true;
    textures.insert({key, newTexture});

    return newTexture;
}

Texture*
TextureManager::GetTexture(Color color_)
{
    std::string key = ColorToString(color_);

    auto textureIt = textures.find(key);
    if(textureIt != textures.end())
        return textureIt->second;

    Texture* newTexture = new Texture{color_};
    newTexture->managerOwners = true;
    textures.insert({key, newTexture});

    return newTexture;
}

Texture*
TextureManager::GetDefaultTexture()
{
    return defaultTexture;
}

//----------------------------------------//
