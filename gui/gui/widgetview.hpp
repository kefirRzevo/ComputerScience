#include "widget.hpp"


//----------------------------------------//

class WidgetView
{
    protected:

        Widget* widget;

    public:

        virtual ~WidgetView();

        Widget*
        GetWidget();

        void
        SetWidget(Widget* widget_);

        virtual bool
        IsInside(Vec2i pos_) const;
        virtual void
        OnRender() const = 0;
};

//----------------------------------------//

class BorderView: virtual public WidgetView
{
    protected:

        int   thickness;
        Color color;

    public:

        BorderView(int thickness_, Color color_);

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
        OnRender() const = 0;
};

//----------------------------------------//

class TextureView: virtual public WidgetView
{
    protected:

        Texture* texture;

    public:

        TextureView(Texture* texture_);

        virtual Texture*
        GetTexture();

        virtual void
        SetTexture(Texture* texture_);

        void
        OnRender() const override;
};

//----------------------------------------//

class ColorView: virtual public WidgetView
{
    protected:

        Color bgColor;

    public:

        ColorView(Color bgColor_);

        Color
        GetColor() const;

        void
        SetColor(Color bgColor_);

        void
        OnRender() const override;
};

//----------------------------------------//

class BorderTextureView: public BorderView, virtual public TextureView
{
    public:

        BorderTextureView(Texture* texture_, int thickness_, Color color_);

        void
        OnRender() const override;
};

//----------------------------------------//

class BorderColorView: public BorderView, public ColorView
{
    public:

        BorderColorView(Color bgColor_, int thickness_, Color color_);

        void
        OnRender() const override;
};

//----------------------------------------//
