#include "../include/mygraphlib.hpp"
#include "../include/config.hpp"

#include <cmath>

//----------------------------------------//

sf::Color
To_SF_Color(Color color)
{
    return sf::Color(color);
}

//----------------------------------------//

bool
Rect::IsInside(const Vec2i& point) const
{
    return point.x >= begin.x && point.y >= begin.y &&
           point.x <= begin.x + size.x && point.y <= begin.y + size.y;
}

//----------------------------------------//

bool
Event::Poll()
{
    return EventManager::Get()->PollEvent(*this);
}

//----------------------------------------//

EventManager*
EventManager::Get()
{
    static std::unique_ptr<EventManager> eventManager;

    if(eventManager.get() == nullptr)
        eventManager.reset(new EventManager());

    return eventManager.get();
}

//----------------------------------------//

void
EventManager::PushEvent(const Event& event)
{
    events.push(event);
}

//----------------------------------------//

bool
EventManager::Poll_SF_Event()
{
    static bool mousePressed = false;
    static Vec2i lastPressed = {};
    sf::Event sfEvent;

    if(!Renderer::Get()->Get_SF_RenderWindow()->pollEvent(sfEvent))
        return false;

    switch (sfEvent.type)
    {
        case sf::Event::Closed:

            events.push({Event::EventType::quit});
            return true;

        case sf::Event::KeyPressed:

            events.push({Event::keyPressed, {sfEvent.key.code}});
            return true;

        case sf::Event::MouseButtonPressed:

            mousePressed = true;
            lastPressed = {sfEvent.mouseButton.x, sfEvent.mouseButton.y};

            if(sfEvent.type != sf::Event::MouseLeft)
                break;
            events.push({Event::mousePressed, {lastPressed}});
            return true;

        case sf::Event::MouseButtonReleased:

            mousePressed = false;

            if(sfEvent.type != sf::Event::MouseLeft)
                break;
            events.push({Event::mouseReleased, {{sfEvent.mouseButton.x, 
                                                 sfEvent.mouseButton.y}}});
            events.push({Event::mouseHovered, {{sfEvent.mouseButton.x, 
                                                sfEvent.mouseButton.y}}});
            return true;

        case sf::Event::MouseMoved:

            if(!mousePressed)
                events.push({Event::mouseHovered, {{sfEvent.mouseButton.x, 
                                                    sfEvent.mouseButton.y}}});
            else
                events.push({Event::mouseMoved, {lastPressed,
                                                 {sfEvent.mouseButton.x, 
                                                  sfEvent.mouseButton.y}}});
            return true;

        default:
            break;
    }
    return false;
}

//----------------------------------------//

bool
EventManager::PollEvent(Event& event)
{
    while(!Poll_SF_Event());

    if(events.empty())
        return false;

    event = events.front();
    events.pop();
    return true;
}

//----------------------------------------//

sf::RenderWindow*
Window::Get_SF_RenderWindow()
{
    return &sfRenderWindow;
}

//----------------------------------------//

Font::Font(const char* path)
{
    if(!sfFont.loadFromFile(path))
        assert(0);
}

//----------------------------------------//

const sf::Font&
Font::Get_SF_Font() const
{
    return sfFont;
}

//----------------------------------------//

Text::Text(const Font& font, const char* text, Color color, size_t size)
{
    sfText.setFont(font.Get_SF_Font());
    sfText.setFillColor(To_SF_Color(color));
    sfText.setString(text);
    sfText.setCharacterSize(size);
}

//----------------------------------------//

const sf::Text&
Text::Get_SF_Text() const
{
    return sfText;
}

//----------------------------------------//

Texture::Texture(const Vec2u& size)
{
    sfTexture.create(size.x, size.y);
}

//----------------------------------------//

Texture::Texture(const char* path)
{
    sfTexture.loadFromFile(path);
}

//----------------------------------------//

Texture::Texture(const char* path, const Vec2u& size)
{
    sf::IntRect sfRect(0, 0, size.x, size.y);
    sfTexture.loadFromFile(path, sfRect);
}

//----------------------------------------//

Texture::Texture(const char* path, const Rect& rect)
{
    sf::IntRect sfRect(rect.begin.x, rect.begin.y, rect.size.x, rect.size.y);
    sfTexture.loadFromFile(path, sfRect);
}

//----------------------------------------//

sf::Texture*
Texture::Get_SF_Texture()
{
    return &sfTexture;
}

//----------------------------------------//

Vec2u
Texture::GetSize() const
{
    return { sfTexture.getSize().x, sfTexture.getSize().y };
}

//----------------------------------------//

const Color*
Texture::ToBuffer() const
{
    sf::Image textureImage = sfTexture.copyToImage();
    return reinterpret_cast<const Color*>(textureImage.getPixelsPtr());
}

//----------------------------------------//

void
Texture::FromBuffer(Color* pixels)
{
    sfTexture.update(reinterpret_cast<sf::Uint8*>(pixels));
}

//----------------------------------------//

TextureManager::TextureManager()
{
    defaultTexture = GetTexture(DEFAULT_TEXTURE_PATH);
    if(!defaultTexture)
    {
        assert(0);
    }
}

//----------------------------------------//

TextureManager::~TextureManager()
{
    for(auto i: textures)
        delete i.second;
}

//----------------------------------------//

TextureManager*
TextureManager::Get()
{
    static std::unique_ptr<TextureManager> textureManager;

    if(textureManager.get() == nullptr)
        textureManager.reset(new TextureManager());

    return textureManager.get();
}

//----------------------------------------//

Texture*
TextureManager::GetTexture(const std::string& path)
{
    auto textureIt = textures.find(path.c_str());

    if(textureIt != textures.end())
        return textureIt->second;

    Texture* newTexture = new Texture(path.c_str());
    if(!newTexture->Get_SF_Texture())
    {
        assert(0);
        delete newTexture;
        return defaultTexture;
    }

    return newTexture;
}

//----------------------------------------//

Texture*
TextureManager::GetDefaultTexture()
{
    return defaultTexture;
}

//----------------------------------------//

Renderer::Renderer(Window* window)
{
    sfRenderWindow = window->Get_SF_RenderWindow();
    sfColor = To_SF_Color(DEFAULT_COLOR);
    thickness = DEFAULT_THICKNESS;
}

//----------------------------------------//

Renderer*
Renderer::Get(Window* sfRenderWindow_)
{
    static std::unique_ptr<Renderer> renderer;

    if(renderer.get() == nullptr)
        renderer.reset(new Renderer(sfRenderWindow_));

    return renderer.get();
}

//----------------------------------------//

sf::RenderWindow*
Renderer::Get_SF_RenderWindow()
{
    return sfRenderWindow;
}

//----------------------------------------//

void
Renderer::SetColor(Color color)
{
    sfColor = To_SF_Color(color);
}

//----------------------------------------//

void
Renderer::SetThickness(float thickness_)
{
    thickness = thickness_;
}

//----------------------------------------//

void
Renderer::DrawPixel(const Vec2f& pos)
{
    sf::CircleShape pixel(1.f, 1);
    pixel.setFillColor(sfColor);
    pixel.move(pos.x, pos.y);
    sfRenderWindow->draw(pixel);
}

//----------------------------------------//

void
Renderer::DrawCircle(const Vec2f& center, float radius)
{
    sf::CircleShape circle(radius);
    circle.setFillColor(sfColor);
    circle.move(center.x, center.y);
    sfRenderWindow->draw(circle);
}

//----------------------------------------//

void
Renderer::DrawThickLine(const Vec2f& point, float degreeAngle, float length)
{
    sf::RectangleShape line({length, thickness});
    line.move(point.x, point.y);
    line.rotate(degreeAngle);
    sfRenderWindow->draw(line);
}

//----------------------------------------//

void
Renderer::DrawThickLineSlow(const Vec2f& p1, const Vec2f& p2)
{
    float length = std::sqrt((p1.x - p2.x) * (p1.x - p2.x) + 
                             (p1.y - p2.y) * (p1.y - p2.y));
    sf::RectangleShape line({length, thickness});
    line.move(p1.x, p1.y);
    line.rotate(180 / PI * atan((p1.y - p2.y) / (p1.x - p2.x)));
    sfRenderWindow->draw(line);
}

//----------------------------------------//

void
Renderer::DrawLine(const Vec2f& p1, const Vec2f& p2)
{
    sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f(p1.x, p1.y), sfColor),
        sf::Vertex(sf::Vector2f(p2.x, p2.y), sfColor)
    };
    sfRenderWindow->draw(line, 2, sf::Lines);
}

//----------------------------------------//

void
Renderer::DrawRect(const Rect& rect)
{
    sf::RectangleShape rect_({static_cast<float>(rect.size.x), 
                              static_cast<float>(rect.size.y)});
    rect_.setFillColor(sfColor);
    rect_.move(rect.begin.x, rect.begin.y);
    sfRenderWindow->draw(rect_);
}

//----------------------------------------//

void
Renderer::DrawTexture(Texture* src, const Rect& dstRect)
{
    sf::Texture* texture =  src->Get_SF_Texture();
    sf::Sprite sprite(*texture);
    sprite.setScale(dstRect.size.x / sprite.getLocalBounds().width,
                    dstRect.size.y / sprite.getLocalBounds().height);
    sprite.move(dstRect.begin.x, dstRect.begin.y);
    sfRenderWindow->draw(sprite);
}

//----------------------------------------//

void
Renderer::DrawTexture(Texture* src, const Rect& srcRect, const Rect& dstRect)
{
    sf::Texture* texture =  src->Get_SF_Texture();
    sf::IntRect sfSrcRect = {srcRect.begin.x, srcRect.begin.y, 
                             srcRect.size.y,  srcRect.size.y};
    sf::Sprite sprite(*texture, sfSrcRect);
    sprite.setScale(dstRect.size.x / sprite.getLocalBounds().width,
                    dstRect.size.y / sprite.getLocalBounds().height);
    sprite.move(dstRect.begin.x, dstRect.begin.y);
    sfRenderWindow->draw(sprite);
}

//----------------------------------------//

void
Renderer::DrawText(const Vec2f& pos, const Text& text)
{
    sf::Text sfTextNew = text.Get_SF_Text();
    sfTextNew.move(pos.x, pos.y);
    sfRenderWindow->draw(sfTextNew);
}

//----------------------------------------//

bool
Renderer::OnRender() const
{
    return sfRenderWindow->isOpen();
}

//----------------------------------------//

void
Renderer::Clear()
{
    sfRenderWindow->clear(sfColor);
}

//----------------------------------------//

void
Renderer::Display()
{
    sfRenderWindow->display();
}

//----------------------------------------//

void
Renderer::Close()
{
    sfRenderWindow->close();
}

//----------------------------------------//
