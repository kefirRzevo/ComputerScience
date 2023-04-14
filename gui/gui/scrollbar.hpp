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

        std::unique_ptr<ScrollBarResponse>  response;
        std::unique_ptr<ScrollBox>          box;


        float minValue;
        float maxValue;
        float curValue;

    public:

        ScrollBar(Vec2i size_, Texture* texture_,
        ScrollBarResponse* responce_, ScrollBox* box_);

        float
        GetCurValue() const;
        float
        GetUnit() const;

        virtual void
        SetValue(Vec2i boxPos_) = 0;

};

class HorScrollBar: public ScrollBar
{
    public:

        HorScrollBar(Vec2i size_, Texture* texture_,
        ScrollBarResponse* responce_, ScrollBox* box_);

        void
        Move(Vec2i delta_) override;
        bool
        OnEvent(const Event& event_) override;
        void
        SetValue(Vec2i boxPos_) override{}
};

//----------------------------------------//
