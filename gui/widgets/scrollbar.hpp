#pragma once

#include "widget.hpp"

class ScrollBarResponse;
class ScrollBox;
class ScrollBar;
class HorScrollBar;
class VerScrollBar;

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

        Texture* release;
        Texture* hover;
        Texture* move;

        ScrollBar* scrollBar;
        bool  pressed;
        Vec2i clickPos;
        float initValue;

        void
        Move(Vec2i pos_);
        void
        SetRectangle(RectInt rect_);

    public:

        ScrollBox(Layout* layout_, Texture* release_,
        Texture* hover_ = nullptr, Texture* move_ = nullptr);

        void
        SetScrollBar(ScrollBar* scrollBar_);

        bool
        ProcessListenerEvent(const Event& event_) override;
        bool
        OnEvent(const Event& event_) override;

    friend class ScrollBar;
    friend class HorScrollBar;
    friend class VerScrollBar;
};

class ScrollBar: public Widget
{
    protected:

        std::shared_ptr<ScrollBarResponse> response;
        ScrollBox* box;

        float value;
        float scaleStep;
        int   scaleLen;

        virtual void
        CalculateValue(float initValue_, Vec2i delta_) = 0;
        virtual RectInt
        GetBoxRectangle() = 0;
        virtual void
        SetScaleParams() = 0;

    public:

        ScrollBar(Layout* layout_, Texture* texture_,
        ScrollBox* box_, ScrollBarResponse* responce_);

        float
        GetValue() const;
        float
        GetScaleStep() const;
        int
        GetScaleLength() const;

        virtual void
        SetBoxLength(int length_) = 0;
        virtual void
        SetValue(float value_) = 0;
        void
        OnLayoutMove() override;
        void
        OnLayoutResize() override;

    friend class ScrollBox;
};

class HorScrollBar: public ScrollBar
{
    protected:

        void
        CalculateValue(float initValue_, Vec2i delta_) override;
        RectInt
        GetBoxRectangle() override;
        void
        SetScaleParams() override;

    public:

        HorScrollBar(Layout* layout_, Texture* texture_,
        ScrollBox* box_, ScrollBarResponse* responce_);

        static HorScrollBar*
        GetDefault(ScrollBarResponse* response_);

        void
        SetBoxLength(int length_) override;
        void
        SetValue(float value_) override;

    friend class ScrollBox;
};

class VerScrollBar: public ScrollBar
{
    protected:

        void
        CalculateValue(float initValue_, Vec2i delta_) override;
        RectInt
        GetBoxRectangle() override;
        void
        SetScaleParams() override;

    public:

        VerScrollBar(Layout* layout_, Texture* texture_,
        ScrollBox* box_, ScrollBarResponse* responce_);

        static VerScrollBar*
        GetDefault(ScrollBarResponse* response_);

        void
        SetBoxLength(int length_) override;
        void
        SetValue(float value_) override;

    friend class ScrollBox;
};

//----------------------------------------//
