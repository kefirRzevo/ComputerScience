#pragma once

#include "widget.hpp"


class ButtonResponse;
class Button;

class OptionButton;

class CloseCommand;
class CloseButton;

//----------------------------------------//

class ButtonResponse
{
    public:

        virtual ~ButtonResponse() = default;

        virtual void
        OnResponse() = 0;
};

class Button: public Widget
{
    protected:

        std::shared_ptr<ButtonResponse> response;
        bool pressed;

        Texture* onRelease;
        Texture* onHover;
        Texture* onPress;

    public:

        Button(Layout* layout_, ButtonResponse* response_, Texture* onRelease_,
        Texture* onHover_ = nullptr, Texture* onPress_ = nullptr);

        static Button*
        GetDefault(const std::string& string_, ButtonResponse* response_);

        bool
        GetPressed() const;

        void
        SetButtonResponse(ButtonResponse* response_);
        bool
        ProcessListenerEvent(const Event& event_) override;
        bool
        OnEvent(const Event& event_) override;
};

//----------------------------------------//

class OptionButton: public Button
{
    protected:

        bool active;

    public:

        OptionButton(Layout* layout_, ButtonResponse* response_, Texture* onRelease_,
        Texture* onHover_ = nullptr, Texture* onPress_ = nullptr);

        static OptionButton*
        GetDefault(const std::string& string_, ButtonResponse* response_);
        bool
        GetActive() const;
        void
        Diactivate();

        bool
        ProcessListenerEvent(const Event& event_) override;
        bool
        OnEvent(const Event& event_) override;  
};

//----------------------------------------//

class CloseCommand: public ButtonResponse
{
    private:

        Widget* widget;

    public:

        CloseCommand(Widget* widget_);

        void
        OnResponse() override;
};

class CloseButton: public Button
{
    public:

        CloseButton(Layout* layout_, Widget* toClose_, Texture* onRelease_,
        Texture* onHover_ = nullptr, Texture* onPres_ = nullptr);

        static CloseButton*
        GetDefault(Widget* toClose_);
};

//----------------------------------------//
