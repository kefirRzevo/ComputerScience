#include "label.hpp"


//----------------------------------------//

Icon::Icon(Layout* layout_, Texture* texture_):
Widget(layout_, texture_)
{}

Icon*
Icon::GetDefault(Texture* texture_)
{
    Layout* layout = new Layout{{Config::defIconWidth, Config::defIconHeight}};
    return new Icon{layout, texture_};
}

bool
Icon::OnEvent(const Event& event_)
{
    assert(&event_);
    return false;
}

//----------------------------------------//

TextIcon::TextIcon(Layout* layout_, Text* text_, Texture* texture_,
int textMaxSize_):
Widget(layout_, texture_), text(text_), fullString(text->GetString()),
textMaxSize(textMaxSize_)
{
    if(fullString.size() > textMaxSize)
        fullString.erase(textMaxSize, fullString.size());
    OnLayoutResize();
}

TextIcon*
TextIcon::GetDefault(const std::string& string)
{
    Vec2i def = {Config::defTextIconWidth,    Config::defTextIconHeight};
    Vec2i min = {Config::defMinTextIconWidth, Config::defTextIconHeight};
    Vec2i max = {Config::defMaxTextIconWidth, Config::defTextIconHeight};
    Layout* layout = new Layout{{def}, 0, 0, min, max};
    return new TextIcon{layout, new Text{string}, nullptr};
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
    if(fullString.size() > textMaxSize)
        fullString.erase(textMaxSize, fullString.size());

    curString = fullString;
    text->SetString(curString);
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
    if(fullString.size() > textMaxSize)
        fullString.erase(textMaxSize, fullString.size());

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
    if(texture)
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
TextLabelResponse* response_, Text* textStyle_, int textMaxSize_):
TextIcon(layout_, textStyle_, texture_, textMaxSize_), responce(response_)
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
