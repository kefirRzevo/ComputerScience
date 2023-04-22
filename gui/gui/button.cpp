#include "button.hpp"


//----------------------------------------//

Command::~Command()
{}

CloseCommand::CloseCommand(Widget* widget_):
widget(widget_)
{}

void
CloseCommand::Execute()
{
    //widget->GetParent()->Detach(widget);
    fprintf(stderr, "press\n");
}

//----------------------------------------//

Button::Button(Layout* layout_, Command* cmd_,
Texture* onRelease_, Texture* onHover_, Texture* onPress_):
Widget(layout_, onRelease_), cmdPtr(cmd_),
onRelease(onRelease_), onHover(onHover_), onPress(onPress_)
{
    if(!onHover)
        onHover = onRelease;
    if(!onPress)
        onPress = onPress_;
}

bool
Button::ProcessListenerEvent(const Event& event_)
{
    if(event_.type == mouseReleased)
    {
        if(event_.IsMouseType() && !layout->IsInside(event_.mouse.pos))
        {
            pressed = false;
            texture = onRelease;
            return false;
        }
    }
    if(event_.type == mouseHovered)
    {
        system->Unsubscribe(mouseHovered);
        if(!pressed)
            texture = onRelease;
        return false;
    }
    return OnEvent(event_);
}

bool
Button::OnEvent(const Event& event_)
{
    if(event_.IsMouseType() && !layout->IsInside(event_.mouse.pos))
        return false;

    switch(event_.type)
    {
        case mousePressed:

            system->Subscribe(this, mouseHovered);
            system->Subscribe(this, mouseReleased);
            pressed = true;
            texture = onPress;
            break;

        case mouseHovered:

            system->Subscribe(this, mouseHovered);
            if(!pressed)
                texture = onHover;
            break;

        case mouseReleased:

            if(pressed)
                cmdPtr->Execute();
            pressed = false;
            texture = onRelease;
            break;

        default:

            break;
    }

    return true;
}

bool
Button::GetPressed() const
{
    return pressed;
}

//----------------------------------------//
