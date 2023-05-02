#include "renderer.hpp"
#include "../config.hpp"

std::unique_ptr<Renderer> Renderer::renderer;

//----------------------------------------//

Window::Window(Vec2u size_, const std::string& name_):
sfRenderWindow(sf::VideoMode(size_.x, size_.y), name_)
{}

//----------------------------------------//

Renderer::Renderer(Window* window):
sfRenderWindow(&window->sfRenderWindow), sfRenderTarget(sfRenderWindow)
{}

void
Renderer::Initialize(Window* window_)
{
    renderer.reset(new Renderer{window_});
}

Renderer*
Renderer::Get()
{
    return renderer.get();
}

void
Renderer::SetColor(Color color_)
{
    sfColor = To_SF_Color(color_);
}

void
Renderer::DrawPixel(Vec2i pos)
{
    sf::CircleShape pixel{1.f};
    pixel.setFillColor(sfColor);
    pixel.move(static_cast<float>(pos.x), static_cast<float>(pos.y));
    sfRenderTarget->draw(pixel);
}

void
Renderer::DrawPixel(Texture* src, Vec2i pos)
{
    sf::RenderTexture sfRenderTexture{};
    sfRenderTexture.create(src->GetWidth(), src->GetHeight());
    sf::Sprite srcSfSprite{src->sfSrcTexture};
    sfRenderTexture.draw(srcSfSprite);

    sfRenderTarget = &sfRenderTexture;
    DrawPixel(pos);
    sfRenderTexture.display();
    sfRenderTarget = sfRenderWindow;

    src->sfSrcTexture = sfRenderTexture.getTexture();
}

void
Renderer::DrawCircle(Vec2i center, int radius)
{
    sf::CircleShape circle{static_cast<float>(radius)};
    circle.setPointCount(static_cast<size_t>(4 * radius));
    circle.setFillColor(sfColor);
    circle.move(static_cast<float>(center.x - radius),
                static_cast<float>(center.y - radius));
    sfRenderTarget->draw(circle);
}

void
Renderer::DrawCircle(Texture* src, Vec2i center, int radius)
{
    sf::RenderTexture sfRenderTexture{};
    sfRenderTexture.create(src->GetWidth(), src->GetHeight());
    sf::Sprite srcSfSprite{src->sfSrcTexture};
    sfRenderTexture.draw(srcSfSprite);

    sfRenderTarget = &sfRenderTexture;
    DrawCircle(center, radius);
    sfRenderTexture.display();
    sfRenderTarget = sfRenderWindow;

    src->sfSrcTexture = sfRenderTexture.getTexture();
}

void
Renderer::DrawThickLine(Vec2i point, float degreeAngle, float length, int thickness)
{
    sf::RectangleShape line{{length, static_cast<float>(thickness)}};
    line.move(static_cast<float>(point.x), static_cast<float>(point.y));
    line.rotate(degreeAngle);
    sfRenderTarget->draw(line);
}

void
Renderer::DrawThickLine(Texture* src, Vec2i point, float degreeAngle, float length, int thickness)
{
    sf::RenderTexture sfRenderTexture{};
    sfRenderTexture.create(src->GetWidth(), src->GetHeight());
    sf::Sprite srcSfSprite{src->sfSrcTexture};
    sfRenderTexture.draw(srcSfSprite);

    sfRenderTarget = &sfRenderTexture;
    DrawThickLine(point, degreeAngle, length, thickness);
    sfRenderTexture.display();
    sfRenderTarget = sfRenderWindow;

    src->sfSrcTexture = sfRenderTexture.getTexture();
}

void
Renderer::DrawThickLineSlow(Vec2i p1, Vec2i p2, int thickness)
{
    float x1 = static_cast<float>(p1.x);
    float y1 = static_cast<float>(p1.y);
    float x2 = static_cast<float>(p2.x);
    float y2 = static_cast<float>(p2.y);
    
    float length = std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    sf::RectangleShape line{{length, static_cast<float>(thickness)}};
    line.move(x1, y1);
    line.rotate(180 / PI * atan((y1 - y2) / (x1 - x2)));
    sfRenderTarget->draw(line);
}

void
Renderer::DrawThickLineSlow(Texture* src, Vec2i p1, Vec2i p2, int thickness)
{
    sf::RenderTexture sfRenderTexture{};
    sfRenderTexture.create(src->GetWidth(), src->GetHeight());
    sf::Sprite srcSfSprite{src->sfSrcTexture};
    sfRenderTexture.draw(srcSfSprite);

    sfRenderTarget = &sfRenderTexture;
    DrawThickLineSlow(p1, p2, thickness);
    sfRenderTexture.display();
    sfRenderTarget = sfRenderWindow;

    src->sfSrcTexture = sfRenderTexture.getTexture();
}

void
Renderer::DrawLine(Vec2i p1, Vec2i p2)
{
    sf::Vertex line[] =
    {
        sf::Vertex{sf::Vector2f{static_cast<float>(p1.x),
                                static_cast<float>(p1.y)}, sfColor},
        sf::Vertex{sf::Vector2f{static_cast<float>(p2.x),
                                static_cast<float>(p2.y)}, sfColor}
    };
    sfRenderTarget->draw(line, 2, sf::Lines);
}

void
Renderer::DrawLine(Texture* src, Vec2i p1, Vec2i p2)
{
    sf::RenderTexture sfRenderTexture{};
    sfRenderTexture.create(src->GetWidth(), src->GetHeight());
    sf::Sprite srcSfSprite{src->sfSrcTexture};
    sfRenderTexture.draw(srcSfSprite);

    sfRenderTarget = &sfRenderTexture;
    DrawLine(p1, p2);
    sfRenderTexture.display();
    sfRenderTarget = sfRenderWindow;

    src->sfSrcTexture = sfRenderTexture.getTexture();
}

void
Renderer::DrawRect(const RectInt& rect)
{
    sf::RectangleShape rect_{{static_cast<float>(rect.width), 
                              static_cast<float>(rect.height)}};
    rect_.move(static_cast<float>(rect.left), static_cast<float>(rect.top));
    rect_.setFillColor(sfColor);
    sfRenderTarget->draw(rect_);
}

void
Renderer::DrawRect(Texture* src, const RectInt& rect)
{
    sf::RenderTexture sfRenderTexture{};
    sfRenderTexture.create(src->GetWidth(), src->GetHeight());
    sf::Sprite srcSfSprite{src->sfSrcTexture};
    sfRenderTexture.draw(srcSfSprite);

    sfRenderTarget = &sfRenderTexture;
    DrawRect(rect);
    sfRenderTexture.display();
    sfRenderTarget = sfRenderWindow;

    src->sfSrcTexture = sfRenderTexture.getTexture();
}

void
Renderer::DrawTexture(const Texture* dst)
{
    sf::Sprite dstSprite{dst->sfSrcTexture};
    sf::Vector2f newScale
    {
        static_cast<float>(sfRenderTarget->getSize().x) / dstSprite.getLocalBounds().width,
        static_cast<float>(sfRenderTarget->getSize().x) / dstSprite.getLocalBounds().height
    };

    dstSprite.setScale(newScale);
    sfRenderTarget->draw(dstSprite);
}

void
Renderer::DrawTexture(Texture* src, const Texture* dst)
{
    sf::RenderTexture sfRenderTexture{};
    sfRenderTexture.create(src->GetWidth(), src->GetHeight());
    sf::Sprite srcSfSprite{src->sfSrcTexture};
    sfRenderTexture.draw(srcSfSprite);

    sfRenderTarget = &sfRenderTexture;
    DrawTexture(dst);
    sfRenderTexture.display();
    sfRenderTarget = sfRenderWindow;

    src->sfSrcTexture = sfRenderTexture.getTexture();
}

void
Renderer::DrawTexture(const RectInt& srcRect, const Texture* dst)
{
    sf::Sprite dstSprite{dst->sfSrcTexture};
    sf::Vector2f newScale
    {
        static_cast<float>(srcRect.width)  / dstSprite.getLocalBounds().width,
        static_cast<float>(srcRect.height) / dstSprite.getLocalBounds().height
    };
    dstSprite.setPosition(static_cast<float>(srcRect.left),
                          static_cast<float>(srcRect.top));

    dstSprite.setScale(newScale);
    sfRenderTarget->draw(dstSprite);
}

void
Renderer::DrawTexture(Texture* src, const RectInt& srcRect, const Texture* dst)
{
    sf::RenderTexture sfRenderTexture{};
    sfRenderTexture.create(src->GetWidth(), src->GetHeight());
    sf::Sprite srcSfSprite{src->sfSrcTexture};
    sfRenderTexture.draw(srcSfSprite);

    sfRenderTarget = &sfRenderTexture;
    DrawTexture(srcRect, dst);
    sfRenderTexture.display();
    sfRenderTarget = sfRenderWindow;

    src->sfSrcTexture = sfRenderTexture.getTexture();
}


void
Renderer::DrawTexture(const Texture* dst, const RectInt& dstRect)
{
    assert(dstRect.width && dstRect.height);
    sf::IntRect dstSfRect{dstRect.left, dstRect.top, dstRect.width, dstRect.height};
    sf::Sprite  dstSprite{dst->sfSrcTexture, dstSfRect};
    sf::Vector2f newScale
    {
        static_cast<float>(sfRenderTarget->getSize().x) / dstSprite.getLocalBounds().width,
        static_cast<float>(sfRenderTarget->getSize().x) / dstSprite.getLocalBounds().height
    };

    dstSprite.setScale(newScale);
    sfRenderTarget->draw(dstSprite);   
}

void
Renderer::DrawTexture(Texture* src, const Texture* dst, const RectInt& dstRect)
{
    sf::RenderTexture sfRenderTexture{};
    sfRenderTexture.create(src->GetWidth(), src->GetHeight());
    sf::Sprite srcSfSprite{src->sfSrcTexture};
    sfRenderTexture.draw(srcSfSprite);

    sfRenderTarget = &sfRenderTexture;
    DrawTexture(dst, dstRect);
    sfRenderTexture.display();
    sfRenderTarget = sfRenderWindow;

    src->sfSrcTexture = sfRenderTexture.getTexture();
}

void
Renderer::DrawTexture(const RectInt& srcRect, const Texture* dst, const RectInt& dstRect)
{
    sf::IntRect dstSfRect{dstRect.left, dstRect.top, dstRect.width, dstRect.height};
    sf::Sprite  dstSprite{dst->sfSrcTexture, dstSfRect};
    sf::Vector2f newScale
    {
        static_cast<float>(srcRect.width)  / dstSprite.getLocalBounds().width,
        static_cast<float>(srcRect.height) / dstSprite.getLocalBounds().height
    };
    dstSprite.setPosition(static_cast<float>(srcRect.left),
                          static_cast<float>(srcRect.top));

    dstSprite.setScale(newScale);
    sfRenderTarget->draw(dstSprite);
}

void
Renderer::DrawTexture(Texture* src, const RectInt& srcRect, const Texture* dst, const RectInt& dstRect)
{
    sf::RenderTexture sfRenderTexture{};
    sfRenderTexture.create(src->GetWidth(), src->GetHeight());
    sf::Sprite srcSfSprite{src->sfSrcTexture};
    sfRenderTexture.draw(srcSfSprite);

    sfRenderTarget = &sfRenderTexture;
    DrawTexture(srcRect, dst, dstRect);
    sfRenderTexture.display();
    sfRenderTarget = sfRenderWindow;

    src->sfSrcTexture = sfRenderTexture.getTexture();
}

void
Renderer::DrawText(const Text* text)
{
    sfRenderTarget->draw(text->sfText);
}

void
Renderer::DrawText(Texture* src, const Text* text)
{
    sf::RenderTexture sfRenderTexture{};
    sfRenderTexture.create(src->GetWidth(), src->GetHeight());
    sf::Sprite srcSfSprite{src->sfSrcTexture};
    sfRenderTexture.draw(srcSfSprite);

    sfRenderTarget = &sfRenderTexture;
    DrawText(text);
    sfRenderTexture.display();
    sfRenderTarget = sfRenderWindow;

    src->sfSrcTexture = sfRenderTexture.getTexture();
}

bool
Renderer::OnRender() const
{
    return sfRenderWindow->isOpen();
}

void
Renderer::Clear()
{
    sfRenderTarget->clear(sfColor);
}

void
Renderer::Clear(Texture* src)
{
    sf::RenderTexture sfRenderTexture{};
    sfRenderTexture.create(src->GetWidth(), src->GetHeight());
    sf::Sprite srcSfSprite{src->sfSrcTexture};
    sfRenderTexture.draw(srcSfSprite);

    sfRenderTarget = &sfRenderTexture;
    Clear();
    sfRenderTarget = sfRenderWindow;

    src->sfSrcTexture = sfRenderTexture.getTexture();
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

//----------------------------------------//
