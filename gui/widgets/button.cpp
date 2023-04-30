#include "button.hpp"


//----------------------------------------//

CloseCommand::CloseCommand(Widget* widget_):
widget(widget_)
{}

void
CloseCommand::OnResponse()
{
    assert(widget);
    //widget->GetParent()->Detach(widget);
    fprintf(stderr, "press\n");
}

//----------------------------------------//

Button::Button(Layout* layout_, ButtonResponse* response_,
Texture* onRelease_, Texture* onHover_, Texture* onPress_):
Widget(layout_, onRelease_), response(response_), pressed(false),
onRelease(onRelease_), onHover(onHover_), onPress(onPress_)
{
    if(!onHover)
        onHover = onRelease;
    if(!onPress)
        onPress = onRelease;
}

bool
Button::GetPressed() const
{
    return pressed;
}

void
Button::SetButtonResponse(ButtonResponse* response_)
{
    if(response.unique())
        delete response.get();

    response.reset(response_);
}

bool
Button::ProcessListenerEvent(const Event& event_)
{
    if(event_.type == mouseReleased)
    {
        if(layout->IsInside(event_.mouse.pos))
            if(pressed)
                response->OnResponse();

        pressed = false;
        texture = onRelease;
        system->Unsubscribe(mouseReleased);

        return true;
    }
    else if(event_.type == mouseHovered)
    {
        system->Unsubscribe(mouseHovered);
        if(!pressed)
            texture = onRelease;
    }
    return false;
}

bool
Button::OnEvent(const Event& event_)
{
    if(event_.IsMouseType() && !layout->IsInside(event_.mouse.pos))
        return false;

    switch(event_.type)
    {
        case mousePressed:
        {
            system->Reset();
            system->Subscribe(this, mouseHovered);
            system->Subscribe(this, mouseReleased);
            pressed = true;
            texture = onPress;
            break;
        }
        case mouseHovered:
        {
            system->Subscribe(this, mouseHovered);
            if(!pressed)
                texture = onHover;
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

CloseButton::CloseButton(Layout* layout_, Widget* toClose_,
Texture* onRelease_, Texture* onHover_, Texture* onPres_):
Button(layout_, new CloseCommand{toClose_},
onRelease_, onHover_, onPres_)
{}

//----------------------------------------//
