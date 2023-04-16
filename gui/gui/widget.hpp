#pragma once


#include "../graphlib/graphlib.hpp"
#include "widgetview.hpp"
#include "../config.hpp"

#include <list>


class Widget;
class WidgetView;
class WidgetSystem;

#define $$ fprintf(stderr, "%-4d %s\n", __LINE__, __FILE__);

//----------------------------------------//

class Widget
{
    protected:

        RectInt     location;
        WidgetView* view;

        std::list<Widget*>  children;
        Widget*             parent;
        WidgetSystem*       system;

    public:

        Widget           (             ) = default;
        Widget           (const Widget&) = delete;
        Widget& operator=(const Widget&) = delete;

        Widget(const RectInt& location_, WidgetView* view_);
        Widget(      Vec2i    size_,     WidgetView* view_);

        virtual ~Widget();

        const RectInt&
        GetLocation() const;
        WidgetView*
        GetWidgetView();
        Widget*
        GetParent();
        WidgetSystem*
        GetWidgetSystem();

        virtual void
        Move(Vec2i delta_);
        virtual void
        SetPosition(Vec2i pos_);
        virtual void
        SetSize(Vec2i size_);
        void
        SetWidgetView(WidgetView* view_);
        void
        SetParent(Widget* parent_);
        void
        SetWidgetSystem(WidgetSystem* system_);

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
        IsInside(Vec2i pos_) const;
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
        Render() const;
};

//----------------------------------------//
