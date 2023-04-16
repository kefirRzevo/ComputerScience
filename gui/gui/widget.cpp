#include "widget.hpp"


//----------------------------------------//

Widget::Widget(const RectInt& location_, WidgetView* view_):
location(location_), view(view_), parent(nullptr), system(nullptr)
{
    view->SetWidget(this);
}

Widget::Widget(Vec2i size_, WidgetView* view_):
location(size_), view(view_), parent(nullptr), system(nullptr)
{
    view->SetWidget(this);
}

Widget::~Widget()
{
    delete view;
    parent->Detach(this);

    for(auto child: children)
        delete child;
}

const RectInt&
Widget::GetLocation() const
{
    return location;
}

WidgetView*
Widget::GetWidgetView()
{
    return view;
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
Widget::SetWidgetView(WidgetView* view_)
{
    view = view_;
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
    BorderView* border  = nullptr;

    switch (event_.type)
    {
        case mousePressed:

            system->Subscribe(this, mouseMoved);
            break;

        case mouseMoved:

            pos   = event_.mouse.pos;
            delta = event_.mouse.offset;

            if(border = dynamic_cast<BorderView*>(view))
            {
                if(border->IsInsideBorder(pos))
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
    view->OnRender();
    for(auto it = children.end(); it != children.begin();)
    {
        --it;
        (*it)->Render();
    }
}

bool
Widget::IsInside(Vec2i pos_) const
{
    return view->IsInside(pos_);
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
WidgetSystem::Render() const
{
    root->Render();
}

//----------------------------------------//
