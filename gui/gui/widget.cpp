#include "widget.hpp"


//----------------------------------------//

Widget::Widget(Layout* layout_, Texture* texture_):
layout(layout_), texture(texture_), parent(nullptr), system(nullptr)
{
    layout->SetWidget(this);
}

Widget::~Widget()
{
    delete layout;
    parent->Detach(this);

    for(auto child: children)
        delete child;
}

Layout*
Widget::GetLayout()
{
    return layout;
}

Texture*
Widget::GetTexture()
{
    return texture;
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
Widget::SetLayout(Layout* layout_)
{
    layout = layout_;
}

void
Widget::SetTexture(Texture* texture_)
{
    texture = texture_;
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
    layout->Attach(child_->GetLayout());
    child_->SetParent(this);
    child_->SetWidgetSystem(system);
    children.push_front(child_);
}

void
Widget::Detach(Widget* child_)
{
    auto found = std::find(children.begin(), children.end(), child_);
    if(found != children.end())
    {
        (*found)->GetLayout()->Detach(layout);
        child_->SetParent(nullptr);
        children.erase(found);
    }
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
    layout->OnListenerEvent(event_);

    if(event_.type == mouseReleased)
    {
        system->Unsubscribe(mouseMoved);
        system->Unsubscribe(mouseReleased);
        return true;
    }
    else if(event_.type == mouseMoved)
    {
        return true;
    }
    return false;
}

bool
Widget::OnEvent(const Event& event_)
{
    if(layout->IsInside(event_.mouse.pos) && event_.type == mousePressed)
    {
        layout->OnEvent(event_);
        system->Subscribe(this, mouseMoved);
        system->Subscribe(this, mouseReleased);
        return true;
    }
    return false;
}

void
Widget::Render() const
{
    if(layout->GetBorder())
        RenderBorder();

    Renderer::Get()->DrawTexture(*texture, layout->GetRectangle());
    for(auto it = children.end(); it != children.begin();)
    {
        --it;
        (*it)->Render();
    }
}

void
Widget::OnLayoutMove()
{}

void
Widget::OnLayoutResize()
{}

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

void
Widget::RenderBorder() const
{
    const RectInt& rect = layout->GetRectangle();
    int   thickness     = layout->GetBorder();

    Renderer* rend = Renderer::Get();
    rend->SetColor(Black);
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

    size_t nPoints = static_cast<size_t>(thickness) * 4;

    rend->DrawCircle({rect.left,              rect.top              }, 
                                                    thickness, nPoints);
    rend->DrawCircle({rect.left + rect.width, rect.top              }, 
                                                    thickness, nPoints);
    rend->DrawCircle({rect.left,              rect.top + rect.height}, 
                                                    thickness, nPoints);
    rend->DrawCircle({rect.left + rect.width, rect.top + rect.height}, 
                                                    thickness, nPoints);
}

//----------------------------------------//
