#include "scrollbar.hpp"


ScrollBarResponse::~ScrollBarResponse()
{}

ScrollBarResponseTest::ScrollBarResponseTest()
{}

void
ScrollBarResponseTest::OnResponse(float value_)
{
    fprintf(stderr, "%f\n", value_);
}

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
    if(event_.IsMouseType() && !IsInside(event_.mouse.pos))
        return false;

    if(event_.type == mousePressed)
    {
        system->Subscribe(this, mouseMoved);
        system->Subscribe(this, mouseReleased);
    }

    return true;
}

ScrollBar::ScrollBar(Vec2i size_, Texture* texture_,
ScrollBarResponse* responce_, ScrollBox* box_):
Widget(size_, texture_)
{
    responsePtr.reset(responce_);
    boxPtr.reset(box_);
    boxPtr.get()->SetScrollBar(this);
    Attach(boxPtr.get());
}

ScrollBar::~ScrollBar()
{}

float
ScrollBar::GetValue() const
{
    return curValue;
}

HorScrollBar::HorScrollBar(Vec2i size_, Texture* texture_,
ScrollBarResponse* responce_, ScrollBox* box_):
ScrollBar(size_, texture_, responce_, box_)
{
    int halfBoxWidth = boxPtr.get()->GetLocation().width / 2;
    scaleBeginX = location.left + halfBoxWidth;
    scaleEndX   = location.left + location.width - halfBoxWidth;
}

void
HorScrollBar::SetValue(Vec2i boxPos_)
{
    float value = static_cast<float>(boxPos_.x - scaleBeginX) * GetScaleStep();
    value = value > maxValue ? maxValue : value;
    value = value < minValue ? minValue : value;

    responsePtr.get()->OnResponse(value);
    curValue = value;

    int temp1 = static_cast<int>((value - minValue) / GetScaleStep());
    int temp2 = scaleBeginX - boxPtr.get()->GetLocation().width / 2;

    boxPtr.get()->SetPosition({temp1 + temp2, location.top});
}

float
HorScrollBar::GetScaleStep() const
{
    return (maxValue - minValue) / static_cast<float>(scaleEndX - scaleBeginX);
}

void
HorScrollBar::Move(Vec2i delta_)
{
    scaleBeginX += delta_.x;
    scaleEndX   += delta_.x;

    location.left += delta_.x;
    location.top  += delta_.y;

    for(auto child: children)
        child->Move(delta_);
}

bool
HorScrollBar::OnEvent(const Event& event_)
{
    return false;
}

int
HorScrollBar::GetBeginX() const
{
    return scaleBeginX;
}

int
HorScrollBar::GetEndX() const
{
    return scaleEndX;
}
