#include "label.hpp"


//----------------------------------------//

Icon::Icon(Layout* layout_, Texture* texture_):
Widget(layout_, texture_)
{}

bool
Icon::OnEvent(const Event& event_)
{
    assert(&event_);
    return false;
}

//----------------------------------------//

TextIcon::TextIcon(Layout* layout_, Texture* texture_, Text* text_,
int maxSize_):
Widget(layout_, texture_), text(text_), fullString(text->GetString()), maxSize(maxSize_)
{
    if(fullString.size() > maxSize)
        fullString.erase(maxSize, fullString.size());
    OnLayoutResize();
}

bool
TextIcon::CheckSize(Vec2i size_)
{
    return size_.x < layout->GetRectangle().width &&
           size_.y < layout->GetRectangle().height;
}

void
TextIcon::SetString(const char* string)
{
    fullString = string;
    if(fullString.size() > maxSize)
        fullString.erase(maxSize, fullString.size());
}

void
TextIcon::OnLayoutMove()
{
    const RectInt& rect = layout->GetRectangle();
    Vec2i textSize = text->GetSize();
    text->SetPosition({rect.left + (rect.width  - textSize.x) / 2,
                       rect.top  + (rect.height - textSize.y) / 2});
}

void
TextIcon::OnLayoutResize()
{
    if(fullString.size() > maxSize)
        fullString.erase(maxSize, fullString.size());

    curString = fullString;
    text->SetString(curString);
    while(!CheckSize(text->GetSize()))
    {
        curString.pop_back();
        text->SetString(curString);
    }
    OnLayoutMove();
}

bool
TextIcon::OnEvent(const Event& event_)
{
    assert(&event_);
    return false;
}

void
TextIcon::Render() const
{
    Widget::Render();

    Renderer::Get()->DrawText(text.get());
}

//----------------------------------------//

TextLabelResponse::~TextLabelResponse()
{}

TextLabelResponseTest::TextLabelResponseTest()
{}

void
TextLabelResponseTest::OnResponse(const std::string& string_)
{
    fprintf(stderr, "%s\n", string_.c_str());
}

TextLabel::TextLabel(Layout* layout_, Texture* texture_,
TextLabelResponse* response_, Text* textStyle_, int maxSize_):
TextIcon(layout_, texture_, textStyle_, maxSize_), responce(response_)
{}

bool
TextLabel::ProcessListenerEvent(const Event& event_)
{
    curString = text->GetString();
    if(event_.type == textEntered)
    {
        if(event_.key.key < '1' || event_.key.key > 'z')
            return false;

        char symbol = static_cast<char>(event_.key.key);
        fullString.push_back(symbol);
    }
    else if(event_.type == keyPressed)
    {
        if(event_.key.key == sf::Keyboard::Space)
            fullString.push_back(' ');

        if(event_.key.key == sf::Keyboard::Backspace)
            fullString.pop_back();
    }
    OnLayoutResize();
    return true;
}

bool
TextLabel::OnEvent(const Event& event_)
{
    if(event_.IsMouseType() && !layout->IsInside(event_.mouse.pos))
        return false;

    switch(event_.type)
    {
        case mousePressed:
        {
            system->Reset();
            system->Subscribe(this, textEntered);
            system->Subscribe(this, keyPressed);
            break;
        }
        default:
        {
            break;
        }
    }
    return true;
}

//----------------------------------------//
