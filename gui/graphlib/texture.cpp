#include "texture.hpp"
#include "../config.hpp"


//----------------------------------------//

Font::Font(const char* path)
{
    sfFont.loadFromFile(path);
}

//----------------------------------------//

FontManager::FontManager():
defaultFont(GetFont(Config::defFontPath))
{}

FontManager::~FontManager()
{
    for(auto i: fonts)
        delete &i.second;
}

FontManager*
FontManager::Get()
{
    static std::unique_ptr<FontManager> fontManager;

    if(fontManager.get() == nullptr)
        fontManager.reset(new FontManager{});

    return fontManager.get();
}

const Font&
FontManager::GetFont(const char* path)
{
    auto fontIt = fonts.find(path);

    if(fontIt != fonts.end())
        return fontIt->second;

    Font* newFont = new Font{path};
    fonts.insert({path, *newFont});

    return *newFont;
}

const Font&
FontManager::GetDefaultFont() const
{
    return defaultFont;
}

//----------------------------------------//

Text::Text(const std::string& string_,
const Font& font_, Color color_, int size_)
{
    sfText.setString(string_);
    sfText.setFont(font_.sfFont);
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

Texture::Texture(Vec2i size_):
isManagerOwner(false)
{
    sf::Texture* temp = new sf::Texture{};
    temp->create(static_cast<unsigned int>(size_.x), 
                 static_cast<unsigned int>(size_.y));
    sfSrcTexture  = temp;
    sfTextureRect = {0, 0, size_.x, size_.y};
    sfSprite = sf::Sprite{*sfSrcTexture};
}

Texture::Texture(Color color_):
isManagerOwner(false), sfSrcTexture(nullptr), sfTextureRect({})
{
    sfSprite.setColor(To_SF_Color(color_));
}

Texture::Texture(const char* path):
isManagerOwner(true)
{
    sfSrcTexture  = TextureManager::Get()->GetTexture(path);
    sfTextureRect = {0, 0, static_cast<int>(sfSrcTexture->getSize().x),
                           static_cast<int>(sfSrcTexture->getSize().y)};
    sfSprite = sf::Sprite{*sfSrcTexture};
}

Texture::Texture(const char* path, int i, int j, int w, int h):
isManagerOwner(true)
{
    sfSrcTexture  = TextureManager::Get()->GetTexture(path);

    int width  = static_cast<int>(sfSrcTexture->getSize().x);
    int height = static_cast<int>(sfSrcTexture->getSize().y);

    sfTextureRect = {width * i/w, height * j/h, width / h, height / h};
    sfSprite = sf::Sprite{*sfSrcTexture, sfTextureRect};
}

Texture::Texture(const char* path, const RectInt& rect):
Texture(path)
{
    sfSrcTexture  = TextureManager::Get()->GetTexture(path);
    sfTextureRect = To_SF_Rect(rect);
    sfSprite = sf::Sprite{*sfSrcTexture, sfTextureRect};
}

std::unique_ptr<Color>
Texture::ToBuffer() const
{
    sf::Image sfImage = sfSrcTexture->copyToImage();
    unsigned int numberPixels = sfImage.getSize().x * sfImage.getSize().y;

    const Color* oldPixels = reinterpret_cast<const Color*>
    ((sfImage.getPixelsPtr()), numberPixels);

    Color* newPixels = new Color[numberPixels];
    std::memcpy(newPixels, oldPixels, numberPixels);

    std::unique_ptr<Color> pixelsPtr = std::make_unique<Color>();
    pixelsPtr.reset(newPixels);

    return pixelsPtr;
}

void
Texture::FromBuffer(Color* pixels)
{
    if(!isManagerOwner)
    {
        const_cast<sf::Texture*>(sfSrcTexture)->
        update(reinterpret_cast<sf::Uint8*>(pixels));
    }
    else
    {
        isManagerOwner = false;
        sf::Texture* temp = new sf::Texture{};
        temp->create(sfSrcTexture->getSize().x, sfSrcTexture->getSize().y);
        temp->update(reinterpret_cast<sf::Uint8*>(pixels));
        sfSrcTexture = temp;
    }
    sfSprite = sf::Sprite{*sfSrcTexture, sfTextureRect};
}

void
Texture::FromBuffer(Color* pixels, int w, int h, int x, int y)
{
    if(!isManagerOwner)
    {
        const_cast<sf::Texture*>(sfSrcTexture)->
        update(reinterpret_cast<sf::Uint8*>(pixels), w, h, x, y);
    }
    else
    {
        isManagerOwner = false;
        sf::Texture* temp = new sf::Texture{};
        temp->create(sfSrcTexture->getSize().x, sfSrcTexture->getSize().y);
        temp->update(reinterpret_cast<sf::Uint8*>(pixels), w, h, x, y);
        sfSrcTexture = temp;
    }
    sfSprite = sf::Sprite{*sfSrcTexture, sfTextureRect};
}

Texture::~Texture()
{
    if(!isManagerOwner)
        delete sfSrcTexture;
}

//----------------------------------------//

TextureManager::TextureManager():
defaultSfSrcTexture(GetTexture(Config::defTexturePath))
{}

TextureManager::~TextureManager()
{
    for(auto i: sfSrcTextures)
        delete i.second;
}

TextureManager*
TextureManager::Get()
{
    static std::unique_ptr<TextureManager> textureManager;

    if(textureManager.get() == nullptr)
        textureManager.reset(new TextureManager{});

    return textureManager.get();
}

const sf::Texture*
TextureManager::GetTexture(const char* path)
{
    auto sfTextureIt = sfSrcTextures.find(path);

    if(sfTextureIt != sfSrcTextures.end())
        return sfTextureIt->second;

    sf::Texture* newSfTexture = new sf::Texture{};
    newSfTexture->loadFromFile(path);
    sfSrcTextures.insert({path, newSfTexture});

    return newSfTexture;
}

const sf::Texture*
TextureManager::GetDefaultTexture() const
{
    return defaultSfSrcTexture;
}

//----------------------------------------//
