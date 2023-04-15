#include "widget.hpp"


Border::Border(Widget* widget_, int thickness_, Color color_):
widget(widget_), thickness(thickness_), color(color_)
{}

const Widget*
Border::GetWidget() const
{
    return widget;
}

Color
Border::GetColor() const
{
    return color;
}

int
Border::GetThickness() const
{
    return thickness;
}

void
Border::SetWidget(Widget* widget_)
{
    widget = widget_;
}

void
Border::SetColor(Color color_)
{
    color = color_;
}

void
Border::SetThickness(int thickness_)
{
    thickness = thickness_;
}

bool
Border::IsInside(Vec2i pos_) const
{
    const RectInt& rect = widget->GetLocation();

    RectInt borderCol = {rect.left - thickness, rect.top    -     thickness,
                                     thickness, rect.height + 2 * thickness};

    if(borderCol.IsInside(pos_))
        return true;

    borderCol.left += rect.width + thickness;

    if(borderCol.IsInside(pos_))
        return true;

    RectInt borderRow = {rect.left,  rect.top - thickness,
                         rect.width,            thickness};

    if(borderRow.IsInside(pos_))
        return true;

    borderRow.top += rect.height + thickness;

    if(borderRow.IsInside(pos_))
        return true;

    return false;
}

void
Border::OnRender() const
{
    const RectInt& rect = widget->GetLocation();

    Renderer* rend = Renderer::Get();
    rend->SetColor(color);
    rend->SetThickness(thickness);

    RectInt borderCol = {rect.left - thickness, rect.top, 
                                     thickness, rect.height};
    rend->DrawRect(borderCol);
    borderCol.left += rect.width + thickness;
    rend->DrawRect(borderCol);

    RectInt borderRow = {rect.left, rect.top - thickness, 
                         rect.width,           thickness};
    rend->DrawRect(borderRow);
    borderRow.top += rect.height + thickness;
    rend->DrawRect(borderRow);

    int nPoints = thickness * 4;

    rend->DrawCircle({rect.left,              rect.top              }, thickness, nPoints);
    rend->DrawCircle({rect.left + rect.width, rect.top              }, thickness, nPoints);
    rend->DrawCircle({rect.left,              rect.top + rect.height}, thickness, nPoints);
    rend->DrawCircle({rect.left + rect.width, rect.top + rect.height}, thickness, nPoints);
}

RectInt
Border::OnResize(Vec2i pos_, Vec2i delta_) const
{
    const RectInt& rect = widget->GetLocation();
    RectInt newLocation = rect;

    RectInt borderCol = {rect.left - thickness, rect.top, 
                                     thickness, rect.height};
    if(borderCol.IsInside(pos_))
    {
        newLocation.left  += delta_.x;
        newLocation.width -= delta_.x;
        return newLocation;
    }
    borderCol.left += rect.width + thickness;
    if(borderCol.IsInside(pos_))
    {
        newLocation.width += delta_.x;
        return newLocation;
    }

    RectInt borderRow = {rect.left, rect.top - thickness, 
                         rect.width,           thickness};
    if(borderRow.IsInside(pos_))
    {
        newLocation.top    += delta_.y;
        newLocation.height -= delta_.y;
        return newLocation;
    }
    borderRow.top += rect.height + thickness;
    if(borderRow.IsInside(pos_))
    {
        newLocation.height += delta_.y;
        return newLocation;
    }

    RectInt corner = {rect.left - thickness, rect.top - thickness, 
                                  thickness,            thickness};
    if(corner.IsInside(pos_))
    {
        newLocation.left   += delta_.x;
        newLocation.top    += delta_.y;
        newLocation.width  -= delta_.x;
        newLocation.height -= delta_.y;
        return newLocation;
    }
    corner.left += rect.width + thickness;
    if(corner.IsInside(pos_))
    {
        newLocation.top    += delta_.y;
        newLocation.width  += delta_.x;
        newLocation.height -= delta_.y;
        return newLocation;
    }
    corner.top += rect.height + thickness;
    if(corner.IsInside(pos_))
    {
        newLocation.width  += delta_.x;
        newLocation.height += delta_.y;
        return newLocation;
    }
    corner.left -= rect.width + thickness;
    if(corner.IsInside(pos_))
    {
        newLocation.left   += delta_.x;
        newLocation.width  -= delta_.x;
        newLocation.height += delta_.y;
        return newLocation;
    }
    return newLocation;
}

//----------------------------------------//

Widget::Widget(const RectInt& location_, Texture* texture_, int thickness_, Color color_):
location(location_), parent(nullptr), system(nullptr), activeChild(nullptr),
texture(texture_), border(nullptr)
{
    if(thickness_ > 0)
        border = new Border(this, thickness_, color_);
}

Widget::Widget(Vec2i size_, Texture* texture_, int thickness_, Color color_):
location(size_), parent(nullptr), system(nullptr), activeChild(nullptr),
texture(texture_), border(nullptr)
{
    if(thickness_ > 0)
        border = new Border(this, color_, thickness_);
}

Widget::~Widget()
{
    delete border;
    parent->Detach(this);

    for(auto child: children)
        delete child;
}

const RectInt&
Widget::GetLocation() const
{
    return location;
}

Widget*
Widget::GetParent()
{
    return parent;
}

WidgetSystem*
Widget::GetWidgetSystem()
{
    return system;
}

Widget*
Widget::GetActiveChild()
{
    return activeChild;
}

Texture*
Widget::GetTexture()
{
    return texture;
}

void
Widget::Move(Vec2i delta_)
{
    location.left += delta_.x;
    location.top  += delta_.y;

    for(auto child: children)
        child->Move(delta_);
}

void
Widget::SetPosition(Vec2i pos_)
{
    location.left = pos_.x;
    location.top  = pos_.y;
}

void
Widget::SetSize(Vec2i size_)
{
    location.width  = size_.x;
    location.height = size_.y;
}

void
Widget::SetParent(Widget* parent_)
{
    parent = parent_;
}

void
Widget::SetWidgetSystem(WidgetSystem* system_)
{
    system = system_;

    for(auto child: children)
         child->SetWidgetSystem(system_);
}

void
Widget::SetActiveChild(Widget* child_)
{
    activeChild = child_;
}

void
Widget::SetTexture(Texture* texture_)
{
    if(texture_)
        texture = texture_;
}

void
Widget::Attach(Widget* child_)
{
    children.push_front(child_);
    child_->SetParent(this);
    child_->SetWidgetSystem(system);
    child_->Move({location.left, location.top});
}

void
Widget::Detach(Widget* child_)
{
    auto itFound = std::find(children.begin(), children.end(), child_);
    if(itFound != children.end())
        children.erase(itFound);
}

bool
Widget::ProcessEvent(const Event& event_)
{
    for(auto it = children.begin(); it != children.end(); ++it)
    {
        if((*it)->ProcessEvent(event_))
        {
            if(event_.type == mousePressed)
            {
                Widget* active = *it;
                children.erase(it);
                children.push_front(active);
            }
            return true;
        }
    }
    return OnEvent(event_);
}

bool
Widget::ProcessListenerEvent(const Event& event_)
{
    return OnEvent(event_);
}

bool
Widget::OnEvent(const Event& event_)
{
    if(event_.IsMouseType() && !IsInside(event_.mouse.pos))
        return false;

    Vec2i pos   = {};
    Vec2i delta = {};

    RectInt newLocation = location;

    switch (event_.type)
    {
        case mousePressed:

            system->Subscribe(this, mouseMoved);
            system->Subscribe(this, mouseHovered);
            break;

        case mouseMoved:

            pos   = event_.mouse.pos;
            delta = event_.mouse.offset;
            if(border)
            {
                if(border->IsInside(pos))
                {
                    newLocation = border->OnResize(pos, delta);
                    SetPosition({newLocation.left, newLocation.top});
                    SetSize({newLocation.width, newLocation.height});
                    break;
                }
            }
            Move(delta);
            break;

        default:

            break;
    }

    return true;
}

void
Widget::Render() const
{
    if(border)
        border->OnRender();

    Renderer::Get()->DrawTexture(*texture, location);
    for(auto it = children.end(); it != children.begin();)
    {
        --it;
        (*it)->Render();
    }
}

bool
Widget::IsInside(Vec2i pos_)
{
    if(location.IsInside(pos_))
        return true;

    if(border)
        if(border->IsInside(pos_))
            return true;

    return false;
}

//----------------------------------------//

WidgetSystem::WidgetSystem(Widget* root_):
root(root_), listeners({count, nullptr})
{
    root_->SetWidgetSystem(this);
}

void
WidgetSystem::Subscribe(Widget* listener_, EventType type_)
{
    listeners[type_] = listener_;
}

void
WidgetSystem::Unsubscribe(EventType type_)
{
    listeners[type_] = nullptr;
}

void
WidgetSystem::Reset()
{
    for(auto& listener: listeners)
        listener = nullptr;
}

void
WidgetSystem::ProcessEvent(const Event& event_)
{
    if(event_.type != mousePressed)
        if(listeners[event_.type])
            if(listeners[event_.type]->ProcessListenerEvent(event_))
                return;

    root->ProcessEvent(event_);
}

void
WidgetSystem::Render()
{
    root->Render();
}
