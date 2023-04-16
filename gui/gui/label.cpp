#include "label.hpp"


//----------------------------------------//

Icon::Icon(Vec2i size_, WidgetView* view_):
Widget(size_, view_)
{}

bool
Icon::OnEvent(const Event& event_)
{
    return false;
}

//----------------------------------------//

TextIcon::TextIcon(Vec2i size_, WidgetView* view_, Text* text_):
Widget(size_, view_), text(text_)
{
    Vec2i textSize = text->GetSize();
    text->SetPosition({location.left + (location.width  - textSize.x) / 2,
                       location.top  + (location.height - textSize.y) / 2});
}

void
TextIcon::Move(Vec2i delta_)
{
    location.left += delta_.x;
    location.top  += delta_.y;
    text->Move(delta_);

    for(auto child: children)
        child->Move(delta_);
}

void
TextIcon::SetPosition(Vec2i pos_)
{
    location.left = pos_.x;
    location.top  = pos_.y;

    Vec2i textSize = text->GetSize();
    text->SetPosition({location.left + (location.width  - textSize.x) / 2,
                       location.top  + (location.height - textSize.y) / 2});
}

void
TextIcon::SetSize(Vec2i size_)
{
    location.width = size_.x;
    location.height = size_.y;

    Vec2i textSize = text->GetSize();
    text->SetPosition({location.left + (size_.x - textSize.x) / 2,
                       location.top  + (size_.y - textSize.y) / 2});
}

bool
TextIcon::OnEvent(const Event& event_)
{
    return false;
}

void
TextIcon::Render() const
{
    view->OnRender();
    Renderer::Get()->DrawText(*text);
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

TextLabel::TextLabel(Vec2i size_, WidgetView* view_,
TextLabelResponse* response_, Text* textStyle_):
Widget(size_, view_), responce(response_), text(textStyle_)
{
    Vec2i textSize = text->GetSize();
    text->SetPosition({location.left + (location.width  - textSize.x) / 2,
                       location.top  + (location.height - textSize.y) / 2});
}

bool
TextLabel::CheckSize(Vec2i size_)
{
    return size_.x < location.width && size_.y < location.height;
}

void
TextLabel::Move(Vec2i delta_)
{
    location.left += delta_.x;
    location.top  += delta_.y;
    text->Move(delta_);

    for(auto child: children)
        child->Move(delta_);
}

void
TextLabel::SetPosition(Vec2i pos_)
{
    location.left = pos_.x;
    location.top  = pos_.y;

    Vec2i textSize = text->GetSize();
    text->SetPosition({location.left + (location.width  - textSize.x) / 2,
                      location.top   + (location.height - textSize.y) / 2});
}

void
TextLabel::SetSize(Vec2i size_)
{
    Vec2i textSize = text->GetSize();

    if(!CheckSize(textSize))
    {
        assert(0);
        return;
    }

    location.width  = size_.x;
    location.height = size_.y;

    text->SetPosition({location.left + (location.width  - textSize.x) / 2,
                       location.top  + (location.height - textSize.y) / 2});   
}

bool
TextLabel::ProcessListenerEvent(const Event& event_)
{
    std::string newString = text->GetString();
    if(event_.type == textEntered)
    {
        if(event_.key.key < '1' || event_.key.key > 'z')
            return false;

        char symbol = static_cast<char>(event_.key.key);
        newString.push_back(symbol);
        text->SetString(newString);

        if(!CheckSize(text->GetSize()))
        {
            newString.pop_back();
            text->SetString(newString);
        }
    }
    else if(event_.type == keyPressed)
    {
        if(event_.key.key == sf::Keyboard::Backspace)
        {
            newString.pop_back();
            text->SetString(newString);
        }
    }
    return false;
}

bool
TextLabel::OnEvent(const Event& event_)
{
    if(event_.IsMouseType() && !IsInside(event_.mouse.pos))
        return false;

    switch(event_.type)
    {
        case mousePressed:

            system->Subscribe(this, textEntered);
            system->Subscribe(this, keyPressed);
            break;
        
        default:

            break;
    }
    return true;
}

void
TextLabel::Render() const
{
    view->OnRender();
    Renderer::Get()->DrawText(*text);
}

//----------------------------------------//
