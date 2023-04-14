#pragma once


#include "widget.hpp"


class ScrollBox;
class ScrollBar;

//----------------------------------------//

class ScrollBarResponse
{
    public:

        virtual ~ScrollBarResponse();

        virtual void
        OnResponse(float value_) = 0;
};

class ScrollBarResponseTest: public ScrollBarResponse
{
    public:

        ScrollBarResponseTest();

        void
        OnResponse(float value_) override;
};

class ScrollBox: public Widget
{
    protected:

        ScrollBar* scrollBar;

    public:

        ScrollBox(Vec2i size_, Texture* texture_);

        void
        SetScrollBar(ScrollBar* scrollBar_);
        void
        SetPosition(Vec2i pos_);

        bool
        ProcessListenerEvent(const Event& event_) override;
        bool
        OnEvent(const Event& event_) override; 
};

class ScrollBar: public Widget
{
    protected:

        std::unique_ptr<ScrollBarResponse>  responsePtr;
        std::unique_ptr<ScrollBox>          boxPtr;

        float minValue = 0.f;
        float maxValue = 1.f;
        float curValue;

    public:

        ScrollBar(Vec2i size_, Texture* texture_,
        ScrollBarResponse* responce_, ScrollBox* box_);

        virtual ~ScrollBar();

        float
        GetValue() const;

        virtual float
        GetScaleStep() const = 0;
        virtual void
        SetValue(Vec2i clickPos_) = 0;
};

class HorScrollBar: public ScrollBar
{
    protected:

        int scaleBeginX;
        int scaleEndX;
 
    public:

        HorScrollBar(Vec2i size_, Texture* texture_,
        ScrollBarResponse* responce_, ScrollBox* box_);

        void
        SetValue(Vec2i boxPos_) override;
        float
        GetScaleStep() const override;
        void
        Move(Vec2i delta_) override;
        bool
        OnEvent(const Event& event_) override;

        int
        GetBeginX() const;
        int
        GetEndX() const;
};

//----------------------------------------//
