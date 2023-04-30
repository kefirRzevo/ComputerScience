#include "dropdownlist.hpp"


//----------------------------------------//

ListOpenerCommand::ListOpenerCommand(DropDownList* list_):
list(list_)
{}

void
ListOpenerCommand::OnResponse()
{
    list->ChangeVisibility();
}

//----------------------------------------//

ListOpenerButton::ListOpenerButton(Layout* layout_, DropDownList* list_,
Texture* onRelease_, Texture* onHover_, Texture* onPress_):
Button(layout_, new ListOpenerCommand{list_}, onRelease_, onHover_, onPress_),
list(list_)
{
    list->SetParent(this);
    list->SetWidgetSystem(system);
    children.push_back(list);
}

void
ListOpenerButton::Attach(Widget* child_)
{
    if(child_ == list)
        return;
    else
        Widget::Attach(child_);
}

void
ListOpenerButton::Detach(Widget* child_)
{
    if(child_ == list)
        return;
    else
        Widget::Detach(child_);
}

void
ListOpenerButton::OnLayoutMove()
{
    list->UpdateDropPoint();
}

void
ListOpenerButton::OnLayoutResize()
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

    Widget* widget = parent;
    RectInt container = widget->GetLayout()->GetRectangle();
    while(widget->GetParent() && dynamic_cast<Container*>(widget->GetParent()->GetLayout()))
    {
        widget = widget->GetParent();
        container = widget->GetLayout()->GetRectangle();
    }

    int contAdd = widget->GetLayout()->GetAddition() / 2;
    RectInt contRect = {container.left - contAdd, container.top - contAdd,
                        container.width + 2 * contAdd, container.height + 2 * contAdd};

    const RectInt& rect = layout->GetRectangle();
    int add = layout->GetAddition() / 2;
    RectInt marginRect = {rect.left  -     add, rect.top    -     add,
                          rect.width + 2 * add, rect.height + 2 * add};

    RectInt newRect = rect;

    int width  = static_cast<int>(Config::windowWidth);
    int height = static_cast<int>(Config::windowHeight);

    if(contRect.left + marginRect.width < width &&
        contRect.top + contRect.height + marginRect.height < height)
    {
        newRect.left = contRect.left;
        newRect.top  = contRect.top + contRect.height;
    }
    else if(contRect.left + contRect.width + marginRect.width < width &&
    contRect.top + marginRect.height < height)
    {
        newRect.left = contRect.left + contRect.width;
        newRect.top = contRect.top;
    }
    else if(contRect.left + marginRect.width < width &&
    contRect.top - marginRect.height > 0)
    {
        newRect.left = contRect.left;
        newRect.top = contRect.top - marginRect.height;
    }
    else
    {
        newRect.left = contRect.left - marginRect.width;
        newRect.top = marginRect.top;
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
