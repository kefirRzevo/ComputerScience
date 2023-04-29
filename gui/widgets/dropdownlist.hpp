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

class ButtonListOpener: public Button
{   
    protected:

        DropDownList* list;
    
    public:

        ButtonListOpener(Layout* layout_, DropDownList* list_,
        Texture* onRelease_, Texture* onHover_ = nullptr,
        Texture* onPress_ = nullptr);

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

        bool  visible;
        
    public:

        DropDownList(Column* layout_, Texture* texture_);

        void
        UpdateDropPoint();

        bool
        ProcessEvent(const Event& event_) override;
        bool
        ProcessListenerEvent(const Event& event_) override;
        bool
        OnEvent(const Event& event_) override;

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
