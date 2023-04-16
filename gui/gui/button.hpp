#pragma once


#include "widget.hpp"


class Command;
class Button;
class ButtonView;

//----------------------------------------//

class Command
{
    public:

        virtual ~Command();

        virtual void
        Execute() = 0;
};

class CloseCommand: public Command
{
    protected:

        Widget* widget;

    public:

        CloseCommand(Widget* widget_);

        void Execute() override;
};

//----------------------------------------//

class Button: public Widget
{
    protected:

        std::unique_ptr<Command> cmdPtr;
        bool pressed;

    public:

        Button(Vec2i size_, Command* cmd_, ButtonView* view_);

        bool
        ProcessListenerEvent(const Event& event_) override;
        bool
        OnEvent(const Event& event_) override;

        bool
        GetPressed() const;
        void
        SetPressed(bool pressed_);
};

//----------------------------------------//
