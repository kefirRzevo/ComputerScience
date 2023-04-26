#pragma once

#include "../graphlib/graphlib.hpp"
#include "../config.hpp"
#include "widget.hpp"

#include <list>

class Widget;

class Layout;
class Container;
class Row;
class Column;

class ScrollBox;

enum class BorderPart: char
{
    NoBorder,
    LeftSide,
    RightSide,
    TopSide,
    BottomSide,
    TopLeftCorner,
    TopRightCorner,
    BottomLeftCorner,
    BottomRightCorner,
    count
};

class Layout
{
    public:

        Layout           (              ) = delete;
        Layout           (const Layout& ) = delete;
        Layout& operator=(const Layout& ) = delete;
        Layout           (      Layout&&) = default;
        Layout& operator=(      Layout&&) = default;

        Layout(const RectInt& rect_,
        int margin_ = Config::defMargin, int border_ = Config::defBorder,
        Vec2i minSize_ = {Config::minWidth, Config::minHeight},
        Vec2i maxSize_ = {Config::maxWidth, Config::maxHeight});

        virtual ~Layout() = default;

        Widget*
        GetWidget();
        const RectInt&
        GetRectangle() const;
        int
        GetMargin() const;
        int
        GetBorder() const;
        Vec2i
        GetMinSize() const;
        Vec2i
        GetMaxSize() const;
        int
        GetAddition() const;

        void
        SetWidget(Widget* widget_);
        bool
        IsInside(Vec2i pos_) const;
        bool
        IsInsideBorder(Vec2i pos_) const;
        void
        OnListenerEvent(const Event& event_);
        void
        OnEvent(const Event& event_);

        virtual void
        Attach(Layout* child_);
        virtual void
        Detach(Layout* child_);

    protected:

        std::vector<Layout*> children;
        Layout* parent;
        Widget* widget;

        RectInt rect;
        int     margin;
        int     border;

        Vec2i   minSize;
        Vec2i   maxSize;

        int        addition;
        BorderPart part;
        bool       onResize;
        bool       onMove;

        void
        OnMove(Vec2i delta_);
        virtual void
        OnResize(const RectInt& rect_);
        RectInt
        GetNewRectangle(Vec2i pos_);
        BorderPart
        GetBorderPart(Vec2i pos_);

    friend class Container;
    friend class Row;
    friend class Column;

    friend class ScrollBox;
};

class Container: public Layout
{
    public:

        Container(int margin_ = Config::defMargin,
        int border_ = Config::defBorder);

        void
        Attach(Layout* child_) override = 0;
        void
        Detach(Layout* child_) override = 0;

    protected:
        
        void
        OnResize(const RectInt& rect_) override;

        virtual void
        PlaceChildren() = 0;
};

class Row: public Container
{
    public:

        Row(int margin_ = Config::defMargin,
        int border_ = Config::defBorder);

        void
        Attach(Layout* child_) override;
        void
        Detach(Layout* child_) override;

    protected:

        void
        PlaceChildren() override;
};

class Column: public Container
{
    public:

        Column(int margin_ = Config::defMargin,
        int border_ = Config::defBorder);

        void
        Attach(Layout* child_) override;
        void
        Detach(Layout* child_) override;

    protected:

        void
        PlaceChildren() override;
};
