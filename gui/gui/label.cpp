#include "label.hpp"


TextLabel::TextLabel(Vec2i size_, Color bgColor_,
const char* string_, const Font& font_, Color textColor_, int textSize_):
Widget(size_, nullptr, 0),
text(font_, string_, textColor_, textSize_), bgColor(bgColor_)
{
    Vec2i textSize = text.GetSize();
    text.SetPosition({location.left + (location.width  - textSize.x) / 2,
                      location.top  + (location.height - textSize.y) / 2});
}

void
TextLabel::Move(Vec2i delta_)
{
    location.left += delta_.x;
    location.top  += delta_.y;
    text.Move(delta_);

    for(auto child: children)
        child->Move(delta_);
}

void
TextLabel::SetPosition(Vec2i pos_)
{
    location.left = pos_.x;
    location.top  = pos_.y;

    Vec2i textSize = text.GetSize();
    text.SetPosition({location.left + (location.width  - textSize.x) / 2,
                      location.top  + (location.height - textSize.y) / 2});
}

void
TextLabel::SetSize(Vec2i size_)
{
    location.width  = size_.x;
    location.height = size_.y;

    Vec2i textSize = text.GetSize();
    text.SetPosition({location.left + (location.width  - textSize.x) / 2,
                      location.top  + (location.height - textSize.y) / 2});   
}

void
TextLabel::Render() const
{
    Renderer::Get()->SetColor(bgColor);
    Renderer::Get()->DrawText(text);
}

//----------------------------------------//

Icon::Icon(Vec2i size_, Texture* texture_):
Widget(size_, texture_, 0)
{}

bool
Icon::OnEvent(const Event& event_)
{
    return false;
}