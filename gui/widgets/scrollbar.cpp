#include "scrollbar.hpp"


//----------------------------------------//

ScrollBarResponse::~ScrollBarResponse()
{}

ScrollBarResponseTest::ScrollBarResponseTest()
{}

void
ScrollBarResponseTest::OnResponse(float value_)
{
    fprintf(stderr, "%f\n", value_);
}

//----------------------------------------//

void
ScrollBox::Move(Vec2i pos_)
{
    const RectInt& rect = layout->GetRectangle();
    Vec2i delta = pos_ - Vec2i{rect.left, rect.top};
    layout->OnMove(delta);
}

void
ScrollBox::SetRectangle(RectInt rect_)
{
    layout->OnResize(rect_);
}

ScrollBox::ScrollBox(Layout* layout_,
Texture* release_, Texture* hover_, Texture* move_):
Widget(layout_, release_), release(release_), hover(hover_), move(move_),
scrollBar(nullptr), pressed(false)
{
    if(!hover)
        hover = release_;
    if(!move)
        move = release_;
}

void
ScrollBox::SetScrollBar(ScrollBar* scrollBar_)
{
    scrollBar = scrollBar_;
}

bool
ScrollBox::ProcessListenerEvent(const Event& event_)
{
    if(event_.type == mouseHovered)
    {
        system->Unsubscribe(mouseHovered);
        if(!pressed)
            texture = release;
        return false;
    }
    else if(event_.type == mouseReleased)
    {
        system->Unsubscribe(mouseMoved);
        system->Unsubscribe(mouseHovered);
        system->Unsubscribe(mouseReleased);
        pressed = false;
        texture = release;
        return true;
    }
    else if(event_.type == mouseMoved)
    {
        scrollBar->CalculateValue(initValue, event_.mouse.pos - clickPos);
        return true;
    }
    return false;
}

bool
ScrollBox::OnEvent(const Event& event_)
{
    if(event_.IsMouseType() && !layout->IsInside(event_.mouse.pos))
        return false;

    switch(event_.type)
    {
        case mousePressed:

            system->Subscribe(this, mouseMoved);
            system->Subscribe(this, mouseHovered);
            system->Subscribe(this, mouseReleased);
            clickPos  = event_.mouse.pos;
            initValue = scrollBar->GetValue();
            pressed  = true;
            texture  = move;
            break;

        case mouseHovered:

            system->Subscribe(this, mouseHovered);
            if(!pressed)
                texture = hover;
            break;

        default:

            break;
    }
    return true;
}

//----------------------------------------//

ScrollBar::ScrollBar(Layout* layout_, Texture* texture_,
ScrollBox* box_, ScrollBarResponse* responce_):
Widget(layout_, texture_), response(responce_), box(box_), value(0.f)
{
    box->SetScrollBar(this);
    Attach(box_);
}

float
ScrollBar::GetValue() const
{
    return value;
}

float
ScrollBar::GetScaleStep() const
{
    return scaleStep;
}

int
ScrollBar::GetScaleLength() const
{
    return scaleLen;
}

void
ScrollBar::OnLayoutMove()
{
    SetScaleParams();
}
void
ScrollBar::OnLayoutResize()
{
    SetScaleParams();
    box->SetRectangle(GetBoxRectangle());
}

//----------------------------------------//

HorScrollBar::HorScrollBar(Layout* layout_, Texture* texture_,
ScrollBox* box_, ScrollBarResponse* responce_):
ScrollBar(layout_, texture_, box_, responce_)
{
    SetScaleParams();
    box->SetRectangle(GetBoxRectangle());
}

void
HorScrollBar::CalculateValue(float initValue_, Vec2i delta_)
{
    value = initValue_ + static_cast<float>(delta_.x) * scaleStep;
    value = value > 1.f ? 1.f : value;
    value = value < 0.f ? 0.f : value;

    response->OnResponse(value);
    
    const RectInt& rect = layout->GetRectangle();
    int boxIndent = box->GetLayout()->GetAddition() / 2;
    int left = rect.left + boxIndent + static_cast<int>(value / scaleStep);
    box->Move({left, box->GetLayout()->GetRectangle().top});
}

RectInt
HorScrollBar::GetBoxRectangle()
{
    const RectInt& rect = layout->GetRectangle();
    int boxIndent = box->GetLayout()->GetAddition() / 2;

    int left = rect.left + boxIndent + static_cast<int>(value / scaleStep);
    int top  = rect.top  + boxIndent;
    int width  = box->GetLayout()->GetRectangle().width;
    int height = rect.height - boxIndent * 2;

    return RectInt{left, top, width, height};
}

void
HorScrollBar::SetScaleParams()
{
    const RectInt& rect = layout->GetRectangle();
    int boxIndent = box->GetLayout()->GetAddition() / 2;
    int boxWidth  = box->GetLayout()->GetRectangle().width;

    scaleLen   = rect.width - 2 * boxIndent - boxWidth;
    scaleStep  = 1.f / static_cast<float>(scaleLen);
}

void
HorScrollBar::SetValue(float value_)
{
    value = value_;
    value = value > 1.f ? 1.f : value;
    value = value < 0.f ? 0.f : value;

    response->OnResponse(value);
    
    const RectInt& rect = layout->GetRectangle();
    int boxIndent = box->GetLayout()->GetAddition() / 2;
    int left = rect.left + boxIndent + static_cast<int>(value / scaleStep);
    box->Move({left, box->GetLayout()->GetRectangle().top});
}

//----------------------------------------//

VerScrollBar::VerScrollBar(Layout* layout_, Texture* texture_,
ScrollBox* box_, ScrollBarResponse* responce_):
ScrollBar(layout_, texture_, box_, responce_)
{
    SetScaleParams();
    box->SetRectangle(GetBoxRectangle());
}

void
VerScrollBar::CalculateValue(float initValue_, Vec2i delta_)
{
    value = initValue_ + static_cast<float>(delta_.y) * scaleStep;
    value = value > 1.f ? 1.f : value;
    value = value < 0.f ? 0.f : value;

    response->OnResponse(value);
    
    const RectInt& rect = layout->GetRectangle();
    int boxIndent = box->GetLayout()->GetAddition() / 2;
    int top = rect.top + boxIndent + static_cast<int>(value / scaleStep);
    box->Move({box->GetLayout()->GetRectangle().left, top});
}

RectInt
VerScrollBar::GetBoxRectangle()
{
    const RectInt& rect = layout->GetRectangle();
    int boxIndent = box->GetLayout()->GetAddition() / 2;

    int left = rect.left + boxIndent;
    int top  = rect.top  + boxIndent + static_cast<int>(value / scaleStep);
    int width  = rect.width - boxIndent * 2;
    int height = box->GetLayout()->GetRectangle().height;

    return RectInt{left, top, width, height};
}

void
VerScrollBar::SetScaleParams()
{
    const RectInt& rect = layout->GetRectangle();
    int boxIndent = box->GetLayout()->GetAddition() / 2;
    int boxHeight  = box->GetLayout()->GetRectangle().height;

    scaleLen   = rect.height - 2 * boxIndent - boxHeight;
    scaleStep  = 1.f / static_cast<float>(scaleLen);
}

void
VerScrollBar::SetValue(float value_)
{
    value = value_;
    value = value > 1.f ? 1.f : value;
    value = value < 0.f ? 0.f : value;

    response->OnResponse(value);
    
    const RectInt& rect = layout->GetRectangle();
    int boxIndent = box->GetLayout()->GetAddition() / 2;
    int top = rect.top + boxIndent + static_cast<int>(value / scaleStep);
    box->Move({box->GetLayout()->GetRectangle().left, top});
}

//----------------------------------------//
