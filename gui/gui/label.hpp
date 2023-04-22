#pragma once
#if 0
#include "widget.hpp"


//----------------------------------------//

class Icon: public Widget
{
    public:

        Icon(Vec2i size_, WidgetView* view_);

        bool
        OnEvent(const Event& event_) override;
};

class TextIcon: public Widget
{
    protected:

        std::unique_ptr<Text> text;

    public:

        TextIcon(Vec2i size_, WidgetView* view_, Text* text_);

        void
        Move(Vec2i delta_) override;
        void
        SetPosition(Vec2i pos_) override;
        void
        SetSize(Vec2i size_) override;

        bool
        OnEvent(const Event& event_) override;

        void
        Render() const override;
};

//----------------------------------------//

class TextLabelResponse
{
    public:

        virtual ~TextLabelResponse();

        virtual void
        OnResponse(const std::string& string) = 0;
};

class TextLabelResponseTest: public TextLabelResponse
{
    public:

        TextLabelResponseTest();

        void
        OnResponse(const std::string& string) override;
};

class TextLabel: public Widget
{
    protected:

        std::unique_ptr<TextLabelResponse> responce;
        std::unique_ptr<Text>              text;

    public:

        TextLabel(Vec2i size_, WidgetView* view_,
        TextLabelResponse* response_, Text* textStyle_);

        bool
        CheckSize(Vec2i size_);

        void
        Move(Vec2i delta_) override;
        void
        SetPosition(Vec2i pos_) override;
        void
        SetSize(Vec2i size_) override;

        bool
        ProcessListenerEvent(const Event& event_);
        bool
        OnEvent(const Event& event_) override;

        void
        Render() const override;
};

//----------------------------------------//
#endif