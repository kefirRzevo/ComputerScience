#include "label.hpp"

Label::Label(Vec2i size_, Color bgColor_, const Font& font_,
const char* string_, Color textColor_, int textSize_):
Widget(size_, nullptr, 0, 0),
text(font_, string_, textColor_, textSize_), bgColor(bgColor_)
{
    Vec2i textSize = text.GetSize();
    text.SetPosition({location.left + (location.width  - textSize.x) / 2,
                      location.top  + (location.height - textSize.y) / 2});
}

void
Label::Move(Vec2i delta_)
{
    location.left += delta_.x;
    location.top  += delta_.y;
    text.Move(delta_);

    for(auto child: children)
        child->Move(delta_);
}

void
Label::Resize(const RectInt& location_)
{
    location = location_;
    Vec2i textSize = text.GetSize();
    text.SetPosition({location.left + (location.width  - textSize.x) / 2,
                      location.top  + (location.height - textSize.y) / 2});
}

void
Label::Render() const
{
    Renderer::Get()->SetColor(bgColor);
    Renderer::Get()->DrawRect(location);
    Renderer::Get()->DrawText(text);
}
