#pragma once


#include "../graphlib/graphlib.hpp"
#include "config.hpp"

#include <list>


class Widget;
class Border;
class WidgetSystem;

#define $$ fprintf(stderr, "%-4d %s\n", __LINE__, __FILE__);

//----------------------------------------//

class Border
{
    protected:

        Widget* widget;

        int     thickness;
        Color   color;

    public:

        Border(Widget* widget_, int thickness_, Color color_);

        const Widget*
        GetWidget() const;
        Color
        GetColor() const;
        int
        GetThickness() const;

        void
        SetWidget(Widget* widget_);
        void
        SetColor(Color color_);
        void
        SetThickness(int thickness_);

        bool
        IsInside(Vec2i pos_) const;
        void
        OnRender() const;
        RectInt
        OnResize(Vec2i pos_, Vec2i delta_) const;
};

class Widget
{
    protected:

        RectInt location;

        std::list<Widget*>  children;
        Widget*             parent;
        WidgetSystem*       system;
        Widget*             activeChild;
        
        Texture*    texture;
        Border*     border;

    public:

        Widget           (             ) = default;
        Widget           (const Widget&) = delete;
        Widget& operator=(const Widget&) = delete;

        Widget(const RectInt& location_, Texture* texture_,
        int thickness_ = defaultBorderThickness,
        Color color_ = defaultBorderColor);

        Widget(Vec2i size_, Texture* texture_,
        int thickness_ = defaultBorderThickness,
        Color color_ = defaultBorderColor);

        virtual ~Widget();

        const RectInt&
        GetLocation() const;
        Widget*
        GetParent();
        WidgetSystem*
        GetWidgetSystem();
        Widget*
        GetActiveChild();
        Texture*
        GetTexture();
        //Border*
        //GetBorder();

        virtual void
        Move(Vec2i delta_);
        virtual void
        SetPosition(Vec2i pos_);
        virtual void
        SetSize(Vec2i size_);
        void
        SetParent(Widget* parent_);
        void
        SetWidgetSystem(WidgetSystem* system_);
        void
        SetActiveChild(Widget* child_);
        void
        SetTexture(Texture* texture_);
        //void
        //SetBorder(Border* border_);

        void
        Attach(Widget* child_);
        void
        Detach(Widget* child_);

        virtual bool
        ProcessEvent(const Event& event_);
        virtual bool
        ProcessListenerEvent(const Event& event_);
        virtual bool
        OnEvent(const Event& event_);

        virtual void
        Render() const;
        virtual bool
        IsInside(Vec2i pos_);
};

//----------------------------------------//

class WidgetSystem
{
    protected:

        Widget* root;
        std::vector<Widget*> listeners;

    public:

        WidgetSystem           (                   ) = default;
        WidgetSystem           (const WidgetSystem&) = delete;
        WidgetSystem& operator=(const WidgetSystem&) = delete;

        WidgetSystem(Widget* root_);

        void
        Subscribe(Widget* listener_, EventType type_);
        void
        Unsubscribe(EventType type_);
        void
        Reset();

        void
        ProcessEvent(const Event& event_);
        void
        Render();
};

//----------------------------------------//
