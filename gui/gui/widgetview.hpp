#pragma once


#include "widget.hpp"

class Widget;
class WidgetView;

//----------------------------------------//

class WidgetView
{
    protected:

        Widget*  widget;
        Texture* texture;

    public:

        WidgetView(Texture* texture_);

        virtual ~WidgetView();

        Widget*
        GetWidget();
        Texture*
        GetTexture();

        void
        SetWidget(Widget* widget_);
        void
        SetTexture(Texture* texture_);

        virtual bool
        IsInside(Vec2i pos_) const;
        virtual void
        OnRender() const;
};

//----------------------------------------//

class BorderView: public WidgetView
{
    protected:

        int   thickness;
        Color color;

    public:

        BorderView(Texture* texture_,
        int thickness_ = DEFAULT_BORDER_THICKNESS,
        Color color_   = DEFAULT_BORDER_COLOR);
        
        Color
        GetBorderColor() const;
        int
        GetBorderThickness() const;

        void
        SetBorderColor(Color color_);
        void
        SetBorderThickness(int thickness_);

        bool
        IsInside(Vec2i pos_) const override;
        bool
        IsInsideBorder(Vec2i pos_) const;
        RectInt
        OnResize(Vec2i pos_, Vec2i delta_) const;

        void
        RenderBorder() const;
        void
        OnRender() const override;
};

//----------------------------------------//

class ButtonView: public WidgetView
{
    protected:

        Texture* onRelease;
        Texture* onPress;
        Texture* onHover;

    public:

        ButtonView(Texture* onRelease_, Texture* onPress_, Texture* onHover_);

        Texture*
        GetReleaseTexture();
        Texture*
        GetPressTexture();
        Texture*
        GetHoverTexture();
};

//----------------------------------------//
