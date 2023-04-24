#pragma once


#include "../graphlib/graphlib.hpp"
#include "../config.hpp"
#include "widgetlayout.hpp"

#include <list>

class Layout;

class Widget;
class WidgetView;
class WidgetSystem;

#define $$ fprintf(stderr, "%-4d %s\n", __LINE__, __FILE__);

//----------------------------------------//

class Widget
{
    protected:

        Layout*         layout;
        Texture*        texture;
        Widget*         parent;
        WidgetSystem*   system;

        std::list<Widget*>  children;
        Color borderColor;

    public:

        Widget           (              ) = delete;
        Widget           (const Widget& ) = delete;
        Widget& operator=(const Widget& ) = delete;
        Widget           (      Widget&&) = default;
        Widget& operator=(      Widget&&) = default;

        Widget(Layout* layout_, Texture* texture_);

        virtual ~Widget();

        Layout*
        GetLayout();
        Texture*
        GetTexture();
        Widget*
        GetParent();
        WidgetSystem*
        GetWidgetSystem();
        Color
        GetBorderColor() const;

        void
        SetLayout(Layout* layout_);
        void
        SetTexture(Texture* texture_);
        void
        SetParent(Widget* parent_);
        void
        SetWidgetSystem(WidgetSystem* system_);
        void
        SetBorderColor(Color borderColor_);

        virtual void
        Attach(Widget* child_);
        virtual void
        Detach(Widget* child_);

        virtual bool
        ProcessEvent(const Event& event_);
        virtual bool
        ProcessListenerEvent(const Event& event_);
        virtual bool
        OnEvent(const Event& event_);
        virtual void
        OnLayoutMove();
        virtual void
        OnLayoutResize();

        virtual void
        Render() const;
        virtual void
        RenderBorder() const;
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
