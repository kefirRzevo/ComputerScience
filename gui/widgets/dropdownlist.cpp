#include "dropdownlist.hpp"


//----------------------------------------//

ListOpenerCommand::ListOpenerCommand(DropDownList* list_):
list(list_)
{}

void
ListOpenerCommand::OnResponse()
{
    fprintf(stderr, "co\n");
    list->ChangeVisibility();
    fprintf(stderr, "%d %d %d %d\n", list->GetLayout()->GetRectangle().left,list->GetLayout()->GetRectangle().top, list->GetLayout()->GetRectangle().width, list->GetLayout()->GetRectangle().height);
}

//----------------------------------------//

ButtonListOpener::ButtonListOpener(Layout* layout_, DropDownList* list_,
Texture* onRelease_, Texture* onHover_, Texture* onPress_):
Button(layout_, new ListOpenerCommand{list_}, onRelease_, onHover_, onPress_),
list(list_)
{
    fprintf(stderr, "%p %p %p\n", onRelease_, onHover_, onPress_);
    list->SetParent(this);
    list->SetWidgetSystem(system);
    children.push_back(list);
}

void
ButtonListOpener::Attach(Widget* child_)
{
    if(child_ == list)
        return;
    else
        Widget::Attach(child_);
}

void
ButtonListOpener::Detach(Widget* child_)
{
    if(child_ == list)
        return;
    else
        Widget::Detach(child_);
}

void
ButtonListOpener::OnLayoutMove()
{
    list->UpdateDropPoint();
}

void
ButtonListOpener::OnLayoutResize()
{
    list->UpdateDropPoint();
}

//----------------------------------------//

DropDownList::DropDownList(Column* layout_, Texture* texture_):
Widget(layout_, texture_), visible(false)
{}

void
DropDownList::UpdateDropPoint()
{
    if(!parent)
        return;

    Widget* widget = parent->GetParent();
    RectInt container = widget->GetLayout()->GetRectangle();
    while(widget && dynamic_cast<Container*>(widget->GetLayout()))
    {
        container = widget->GetLayout()->GetRectangle();
        widget = widget->GetParent();
    }

    const RectInt& rect = layout->GetRectangle();
    RectInt newRect = rect;

    if(container.left + rect.width < Config::windowWidth &&
        container.top + container.height + rect.height < Config::windowHeight)
    {
        newRect.left = container.left;
        newRect.top  = container.top + container.height;
    }
    else if(container.left + container.width + rect.width < Config::windowWidth &&
    container.top + rect.height < Config::windowHeight)
    {
        newRect.left = container.left + container.width;
        newRect.top = container.top;
    }
    else if(container.left + rect.width < Config::windowWidth &&
    container.top - rect.height > 0)
    {
        newRect.left = container.left;
        newRect.top = container.top - rect.height;
    }
    else
    {
        newRect.left = container.left - rect.width;
        newRect.top = rect.top;
    }
    layout->OnResize(newRect);
}

bool
DropDownList::ProcessEvent(const Event& event_)
{
    if(visible)
        return Widget::ProcessEvent(event_);
    return false;
}

bool
DropDownList::ProcessListenerEvent(const Event& event_)
{
    if(visible)
        return Widget::ProcessListenerEvent(event_);
    return false;
}

bool
DropDownList::OnEvent(const Event& event_)
{
    if(visible)
        return Widget::OnEvent(event_);
    return false;
}

void
DropDownList::Render() const
{
    if(visible)
        Widget::Render();
}

void
DropDownList::RenderBorder() const
{
    if(visible)
        Widget::RenderBorder();
}

void
DropDownList::Hide()
{
    visible = false;
}

void
DropDownList::Show()
{
    visible = true;
}

void
DropDownList::ChangeVisibility()
{
    visible ? Hide() : Show();
}

//----------------------------------------//
