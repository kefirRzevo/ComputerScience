#include "renderer.hpp"
#include "../config.hpp"

std::unique_ptr<Renderer> Renderer::renderer;

//----------------------------------------//

Window::Window(Vec2u size_, const std::string& name_):
sfRenderWindow(sf::VideoMode(size_.x, size_.y), name_)
{}

//----------------------------------------//

Renderer::Renderer(Window* window):
sfRenderWindow(&window->sfRenderWindow)
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
Renderer::SetThickness(float thickness_)
{
    thickness = thickness_;
}

void
Renderer::DrawPixel(const Vec2i& pos)
{
    sf::CircleShape pixel{1.f};
    pixel.setFillColor(sfColor);
    pixel.move(static_cast<float>(pos.x), static_cast<float>(pos.y));
    sfRenderWindow->draw(pixel);
}

void
Renderer::DrawCircle(const Vec2i& center, int radius, size_t nPoints)
{
    sf::CircleShape circle{static_cast<float>(radius)};
    circle.setPointCount(nPoints);
    circle.setFillColor(sfColor);
    circle.move(static_cast<float>(center.x - radius),
                static_cast<float>(center.y - radius));
    sfRenderWindow->draw(circle);
}

void
Renderer::DrawThickLine(const Vec2i& point, float degreeAngle, float length)
{
    sf::RectangleShape line{{length, thickness}};
    line.move(static_cast<float>(point.x), static_cast<float>(point.y));
    line.rotate(degreeAngle);
    sfRenderWindow->draw(line);
}

void
Renderer::DrawThickLineSlow(const Vec2i& p1, const Vec2i& p2)
{
    float x1 = static_cast<float>(p1.x);
    float y1 = static_cast<float>(p1.y);
    float x2 = static_cast<float>(p2.x);
    float y2 = static_cast<float>(p2.y);
    
    float length = std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    sf::RectangleShape line{{length, thickness}};
    line.move(x1, y1);
    line.rotate(180 / PI * atan((y1 - y2) / (x1 - x2)));
    sfRenderWindow->draw(line);
}

void
Renderer::DrawLine(const Vec2i& p1, const Vec2i& p2)
{
    sf::Vertex line[] =
    {
        sf::Vertex{sf::Vector2f{static_cast<float>(p1.x),
                                static_cast<float>(p1.y)}, sfColor},
        sf::Vertex{sf::Vector2f{static_cast<float>(p2.x),
                                static_cast<float>(p2.y)}, sfColor}
    };
    sfRenderWindow->draw(line, 2, sf::Lines);
}

void
Renderer::DrawRect(const RectInt& rect)
{
    sf::RectangleShape rect_{{static_cast<float>(rect.width), 
                              static_cast<float>(rect.height)}};
    rect_.move(static_cast<float>(rect.left), static_cast<float>(rect.top));
    rect_.setFillColor(sfColor);
    sfRenderWindow->draw(rect_);
}

void
Renderer::DrawTexture(Texture& src, const RectInt& dstRect)
{
    if(!src.sfSrcTexture)
    {
        sfColor = src.sfSprite.getColor();
        DrawRect(dstRect);
        return;
    }
    
    src.sfSprite.setPosition(static_cast<float>(dstRect.left),
                             static_cast<float>(dstRect.top));

    sf::Vector2f newScale = {
        dstRect.width  / src.sfSprite.getLocalBounds().width,
        dstRect.height / src.sfSprite.getLocalBounds().height };

    if(src.sfSprite.getScale() != newScale)
        src.sfSprite.setScale(newScale);

    sfRenderWindow->draw(src.sfSprite);
}

void
Renderer::DrawText(Text& text_)
{
    sfRenderWindow->draw(text_.sfText);
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

//----------------------------------------//
