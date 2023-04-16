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

Button::Button(Vec2i size_, Command* cmd_, ButtonView* view_):
Widget(size_, view_), cmdPtr(cmd_), pressed(false)
{}

bool
Button::ProcessListenerEvent(const Event& event_)
{
    ButtonView* buttonView = dynamic_cast<ButtonView*>(view);
    if(!buttonView)
        return false;

    if(event_.type == mouseReleased)
    {
        if(event_.IsMouseType() && !IsInside(event_.mouse.pos))
        {
            pressed = false;
            view->SetTexture(buttonView->GetReleaseTexture());
            return false;
        }
    }
    if(event_.type == mouseHovered)
    {
        system->Unsubscribe(mouseHovered);
        if(!pressed)
            view->SetTexture(buttonView->GetReleaseTexture());
        return false;
    }
    return OnEvent(event_);
}

bool
Button::OnEvent(const Event& event_)
{
    if(event_.IsMouseType() && !IsInside(event_.mouse.pos))
        return false;

    ButtonView* buttonView = dynamic_cast<ButtonView*>(view);
    if(!buttonView)
        return false;

    switch(event_.type)
    {
        case mousePressed:

            system->Subscribe(this, mouseHovered);
            system->Subscribe(this, mouseReleased);

            pressed = true;
            view->SetTexture(buttonView->GetPressTexture());
            break;

        case mouseHovered:

            if(!pressed)
                view->SetTexture(buttonView->GetHoverTexture());

            system->Subscribe(this, mouseHovered);
            break;

        case mouseReleased:

            if(pressed)
                cmdPtr->Execute();

            pressed = false;
            view->SetTexture(buttonView->GetReleaseTexture());
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

//----------------------------------------//
