#include "dropdownlist.hpp"
#include "label.hpp"


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

ListOpenerButton*
ListOpenerButton::GetDefault(const std::string& string_, DropDownList* list_)
{
    Texture* rel   = Config::defUpReleaseTexture;
    Texture* hov   = Config::defUpHoverTexture;
    Texture* press = Config::defUpPressTexture;

    ListOpenerButton* btn = new ListOpenerButton{new Row{}, list_, rel, hov, press};
    btn->Attach(TextIcon::GetDefault(string_));
    return btn;
}

bool
ListOpenerButton::GetListVisibility() const
{
    return list->GetVisibility();
}

void
ListOpenerButton::HideList()
{
    list->Hide();
}

void
ListOpenerButton::ShowList()
{
    list->Show();
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

DropDownList::DropDownList(Texture* texture_):
Widget(new Column{0, 0}, texture_), activeChild(nullptr), visible(false)
{}

DropDownList*
DropDownList::GetDefault()
{
    return new DropDownList{Config::defDownReleaseTexture};
}

bool
DropDownList::GetVisibility() const
{
    return visible;
}

void
DropDownList::UpdateDropPoint()
{
    if(!parent)
        return;

    RectInt cont = parent->GetLayout()->GetRectangle();

    int contAdd = parent->GetLayout()->indent;
    RectInt contRect = {cont.left  - contAdd,     cont.top    -     contAdd,
                        cont.width + 2 * contAdd, cont.height + 2 * contAdd};

    const RectInt& rect = layout->GetRectangle();
    int add = layout->indent;
    RectInt marginRect = {rect.left  -     add, rect.top    -     add,
                          rect.width + 2 * add, rect.height + 2 * add};

    RectInt newRect = rect;

    int width  = Config::defWindowWidth;
    int height = Config::defWindowHeight;

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
    {
        for(auto it = children.begin(); it != children.end(); ++it)
        {
            if((*it)->ProcessEvent(event_))
            {
                OptionButton* processedChild = dynamic_cast<OptionButton*>(*it);
                if(processedChild && processedChild->GetActive())
                {
                    if(processedChild != activeChild)
                    {
                        activeChild->Diactivate();
                        activeChild = processedChild;
                    }
                }

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
DropDownList::Clear()
{
    for(auto it = children.begin(); it != children.end(); ++it)
    {
        if(dynamic_cast<OptionButton*>(*it))
        {
            Detach(*it);
            delete *it;
        }
    }
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
