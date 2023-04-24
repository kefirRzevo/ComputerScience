#pragma once


#include "widget.hpp"


class Command;
class Button;

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
    private:

        Widget* widget;

    public:

        CloseCommand(Widget* widget_);

        void
        Execute() override;
};

//----------------------------------------//

class Button: public Widget
{
    protected:

        std::unique_ptr<Command> cmdPtr;
        bool pressed;

        Texture* onRelease;
        Texture* onHover;
        Texture* onPress;

    public:

        Button(Layout* layout_, Command* cmd_, Texture* onRelease_,
        Texture* onHover_ = nullptr, Texture* onPress = nullptr);

        bool
        ProcessListenerEvent(const Event& event_) override;
        bool
        OnEvent(const Event& event_) override;

        bool
        GetPressed() const;
};

//----------------------------------------//
