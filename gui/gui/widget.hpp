#pragma once


#include "../graphlib/graphlib.hpp"
#include "widgetlayout.hpp"
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

        Layout*         layout;
        Texture*        texture;
        Widget*         parent;
        WidgetSystem*   system;

        std::list<Widget*>  children;


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

        void
        SetLayout(Layout* layout_);
        void
        SetTexture(Texture* texture_);
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
