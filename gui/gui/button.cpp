#include "button.hpp"

Command::~Command()
{}

CloseCommand::CloseCommand(Widget* widget_):
widget(widget_)
{}

void
CloseCommand::Execute()
{
    widget->GetParent()->Detach(widget);
    fprintf(stderr, "press\n");
}

//----------------------------------------//

Button::Button(Vec2i size_, Command* cmd_,
Texture* onRelease_, Texture* onPress_, Texture* onHover_):
Widget(size_, onRelease_), pressed(false),
onRelease(onRelease_), onPress(onPress_), onHover(onHover_)
{
    cmdPtr.reset(cmd_);
}

bool
Button::ProcessListenerEvent(const Event& event_)
{
    if(event_.type == mouseReleased)
    {
        if(event_.IsMouseType() && !IsInside(event_.mouse.pos))
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
    if(event_.IsMouseType() && !IsInside(event_.mouse.pos))
        return false;

    switch(event_.type)
    {
        case mousePressed:

            system->Reset();
            system->Subscribe(this, mouseHovered);
            system->Subscribe(this, mouseReleased);

            pressed = true;
            texture = onPress;
            break;

        case mouseHovered:

            if(!pressed)
                texture = onHover;

            system->Subscribe(this, mouseHovered);
            break;

        case mouseReleased:

            if(pressed)
                cmdPtr.get()->Execute();

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

void
Button::SetPressed(bool pressed_)
{
    pressed = pressed_;
}


Texture*
Button::GetOnReleaseTexture()
{
    return onRelease;
}

Texture*
Button::GetOnPressTexture()
{
    return onPress;
}

Texture*
Button::GetOnHoverTexture()
{
    return onHover;
}
