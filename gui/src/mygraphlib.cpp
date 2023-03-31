#include "../include/mygraphlib.hpp"
#include "../include/config.hpp"

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
    sfFont.loadFromFile(path);
}

//----------------------------------------//

const sf::Font*
Font::Get_SF_Font() const
{
    return &sfFont;
}

//----------------------------------------//

Text::Text(const Font& font, const char* text, Color color, size_t size)
{
    sfText.setFont(*font.Get_SF_Font());
    //sfText.setColor(To_SF_Color(color));
    sfText.setString(text);
    sfText.setCharacterSize(size);
}

//----------------------------------------//

sf::Text*
Text::Get_SF_Text()
{
    return &sfText;
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
    sfColor = sf::Color::Black;
}

Renderer*
Renderer::Get(Window* sfRenderWindow_)
{
    static std::unique_ptr<Renderer> renderer;

    if(renderer.get() == nullptr)
        renderer.reset(new Renderer(sfRenderWindow_));

    return renderer.get();
}

sf::RenderWindow*
Renderer::Get_SF_RenderWindow()
{
    return sfRenderWindow;
}

void
Renderer::SetColor(Color color)
{
    sfColor = To_SF_Color(color);
}

void
Renderer::DrawPixel(const Vec2i& pos)
{
    sf::CircleShape pixel(1, 1);
    pixel.move({static_cast<float>(pos.x), static_cast<float>(pos.y)});
    sfRenderWindow->draw(pixel);
}

void
Renderer::DrawCircle(const Vec2i& center, int radius)
{
    sf::CircleShape circle(radius);
    circle.getFillColor(color);
}

bool
Renderer::OnRender() const
{
    return sfRenderWindow->isOpen();
}

void
Renderer::Clear()
{
    sfRenderWindow->clear(sfColor);
}

void
Renderer::Display()
{
    sfRenderWindow->display();
}

void
Renderer::Close()
{
    sfRenderWindow->close();
}
