#include "dropdownlist.hpp"


//----------------------------------------//

ListOpenerCommand::ListOpenerCommand(DropDownList* list_):
list(list_)
{}

void
ListOpenerCommand::Execute()
{
    list->ChangeVisibility();
}

DropDownList::DropDownList(Column* layout_, Texture* texture_):
Widget(layout_, texture_), visible(false)
{}

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
