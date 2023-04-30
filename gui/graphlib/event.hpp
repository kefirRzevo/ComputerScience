#pragma once


#include "math.hpp"

#include <queue>


//----------------------------------------//

enum EventType
{
    quit,
    keyPressed,
    textEntered,
    mousePressed,
    rightButtonPress,
    mouseReleased,
    mouseHovered,
    mouseMoved,

    count
};

class Event
{
    public:

        struct KeyValue
        {
            int key;

            KeyValue() = default;

            KeyValue(int key_):
                key(key_) {}
        };

        struct MouseValue
        {
            Vec2i pos;
            Vec2i offset;

            MouseValue() = default;

            MouseValue(Vec2i pos_):
                pos(pos_) {}
            
            MouseValue(Vec2i pos_, Vec2i offset_):
                pos(pos_), offset(offset_) {}
        };

        EventType type;
        MouseValue mouse;
        KeyValue key;

        Event() = default;

        Event(EventType type_):
            type(type_) {}

        Event(EventType type_, KeyValue key_):
            type(type_), key(key_) {}

        Event(EventType type_, MouseValue mouse_):
            type(type_), mouse(mouse_) {}

        bool IsMouseType() const;
};

//----------------------------------------//

class EventManager
{
    private:

        std::queue<Event> events;

        EventManager() = default;

        bool Poll_SF_Event();

    public:

        static EventManager*
        Get();

        void
        PushEvent(const Event& event);

        bool
        PollEvent(Event& event);
};

//----------------------------------------//
