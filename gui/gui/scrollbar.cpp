#include "scrollbar.hpp"


ScrollBarResponse::~ScrollBarResponse()
{}

ScrollBox::ScrollBox(Vec2i size_, Texture* texture_):
Widget(size_, texture_, 0, 0), scrollBar(nullptr)
{}

void
ScrollBox::SetScrollBar(ScrollBar* scrollBar_)
{
    scrollBar = scrollBar_;
}

void
ScrollBox::SetPosition(Vec2i pos_)
{
    Vec2i delta = pos_ - Vec2i{location.left, location.top};
    Move(delta);
}

bool
ScrollBox::ProcessListenerEvent(const Event& event_)
{
    switch(event_.type)
    {
        case mousePressed:

        case mouseMoved:

            scrollBar->SetValue(event_.mouse.pos);
            break;

        case mouseReleased:

            system->Unsubscribe(mousePressed);
            system->Unsubscribe(mouseMoved);
            break;

        default:

            break;
    }
    return true;
}

bool
ScrollBox::OnEvent(const Event& event_)
{
    if(event_.type == mousePressed)
    {
        system->Subscribe(this, mouseMoved);
        system->Subscribe(this, mouseReleased);
    }
}