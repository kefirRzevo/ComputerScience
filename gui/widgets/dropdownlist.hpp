#pragma once

#include "widget.hpp"
#include "button.hpp"


class ListOpenerCommand;
class DropDownList;

//----------------------------------------//

class ListOpenerCommand: public ButtonResponse
{
    private:

        DropDownList* list;

    public:

        ListOpenerCommand(DropDownList* list_);

        void
        OnResponse() override;
};

//----------------------------------------//

class ListOpenerButton: public Button
{   
    protected:

        DropDownList* list;
    
    public:

        ListOpenerButton(Layout* layout_, DropDownList* list_,
        Texture* onRelease_, Texture* onHover_ = nullptr,
        Texture* onPress_ = nullptr);

        static ListOpenerButton*
        GetDefault(const std::string& string_, DropDownList* list_);
        bool
        GetListVisibility() const;

        void
        HideList();
        void
        ShowList();

        void
        Attach(Widget* child_) override;
        void
        Detach(Widget* child_) override;

        void
        OnLayoutMove() override;
        void
        OnLayoutResize() override;
};

//----------------------------------------//

class DropDownList: public Widget
{
    protected:

        OptionButton* activeChild;
        bool visible;
        
    public:

        DropDownList(Texture* texture_);

        static DropDownList*
        GetDefault();
        bool
        GetVisibility() const;

        void
        UpdateDropPoint();

        bool
        ProcessEvent(const Event& event_) override;
        bool
        ProcessListenerEvent(const Event& event_) override;
        bool
        OnEvent(const Event& event_) override;

        void
        Clear();

        void
        Render() const override;
        void
        RenderBorder() const override;

        void
        Hide();
        void
        Show();
        void
        ChangeVisibility();
};

//----------------------------------------//
