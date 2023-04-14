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
    private:

        Widget* widget;

    public:

        CloseCommand(Widget* widget_);

        void Execute() override;
};

//----------------------------------------//

class Button: public Widget
{
    protected:

        bool pressed;
        std::unique_ptr<Command> cmdPtr;

        Texture* onRelease;
        Texture* onPress;
        Texture* onHover;

    public:

        Button(Vec2i size_, Command* cmd_,
        Texture* onRelease_, Texture* onPress_, Texture* onHover_);

        bool
        ProcessListenerEvent(const Event& event_) override;
        bool
        OnEvent(const Event& event_) override;

        bool
        GetPressed() const;
        void
        SetPressed(bool pressed_);

        Texture*
        GetOnReleaseTexture();
        Texture*
        GetOnPressTexture();
        Texture*
        GetOnHoverTexture();
};

//----------------------------------------//
