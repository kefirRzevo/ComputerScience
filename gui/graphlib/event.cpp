#include "event.hpp"
#include "renderer.hpp"

#include <SFML/Graphics.hpp>
#include <iostream>


#include <fstream>
//std::ofstream ("output.txt");
#define fout  std::cout

//----------------------------------------//

bool
Event::IsMouseType() const
{
    if(type == mousePressed  || type == mouseReleased ||
       type == mouseHovered  || type == mouseMoved    ||
       type == rightButtonPress)
        return true;

    return false;
}

//----------------------------------------//

EventManager*
EventManager::Get()
{
    static std::unique_ptr<EventManager> eventManager;

    if(eventManager.get() == nullptr)
        eventManager.reset(new EventManager{});

    return eventManager.get();
}

//----------------------------------------//

void
EventManager::PushEvent(const Event& event)
{
    events.push(event);
}

//----------------------------------------//

bool
EventManager::Poll_SF_Event()
{
    static bool pressed = false;
    static Vec2i prevPos;

    sf::Event sfEvent;

    if(!Renderer::Get()->sfRenderWindow->pollEvent(sfEvent))
        return false;

    switch (sfEvent.type)
    {
        case sf::Event::Closed:
        {
            events.push({quit});
            return true;
        }
        case sf::Event::KeyPressed:
        {
            events.push({keyPressed, Event::KeyValue{sfEvent.key.code}});
            return true;
        }
        case sf::Event::TextEntered:
        {
            events.push({textEntered, Event::KeyValue{static_cast<int>(sfEvent.text.unicode)}});
            return true;
        }
        case sf::Event::MouseButtonPressed:
        {
            pressed = true;
            Vec2i lastPos = {sfEvent.mouseButton.x, sfEvent.mouseButton.y};
            prevPos = lastPos;

            if(sfEvent.mouseButton.button == sf::Mouse::Left)
                events.push({mousePressed, Event::MouseValue{lastPos}});
            else if(sfEvent.mouseButton.button == sf::Mouse::Right)
                events.push({rightButtonPress, Event::MouseValue{lastPos}});
            else
                break;
            return true;
        }
        case sf::Event::MouseButtonReleased:
        {
            pressed = false;
            Vec2i lastPos = {sfEvent.mouseButton.x, sfEvent.mouseButton.y};

            events.push({mouseReleased, Event::MouseValue{lastPos}});
            events.push({mouseHovered,  Event::MouseValue{lastPos}});
            return true;
        }
        case sf::Event::MouseMoved:
        {
            Vec2i lastPos = {sfEvent.mouseMove.x, sfEvent.mouseMove.y};
            if(pressed)
            {
                events.push({mouseMoved, Event::MouseValue{prevPos, lastPos - prevPos}});
                prevPos = lastPos;
            }
            events.push({mouseHovered, Event::MouseValue{lastPos}});
            return true;
        }
        default:
        {
            break;
        }
    }
    return false;
}

//----------------------------------------//

bool
EventManager::PollEvent(Event& event)
{
    Poll_SF_Event();

    if(events.empty())
        return false;

    event = events.front();
/*
    switch (event.type)
    {
        case keyPressed:
        {
            fout << "keyPressed ";
            fout << event.key.key;
            break;
        }
        case mousePressed:
        {
            fout << "mousePressed ";
            fout << event.mouse.pos.x << " " << event.mouse.pos.y;
            break;
        }
        case mouseReleased:
        {
            fout << "mouseReleased ";
            fout << event.mouse.pos.x << " " << event.mouse.pos.y;
            break;
        }
        case mouseMoved:
        {
            fout << "mouseMoved ";
            fout << event.mouse.pos.x << " " << event.mouse.pos.y << " & " << event.mouse.offset.x << " " << event.mouse.offset.y;
            break;
        }
        case mouseHovered:
        {
            fout << "mouseHovered ";
            fout << event.mouse.pos.x << " " << event.mouse.pos.y;
            break;
        }
        case rightButtonPress:
        {
            fout << "rightButtonPress ";
            fout << event.mouse.pos.x << " " << event.mouse.pos.y;
            break;
        }
        default:
        {
            fout << "WTF";
            break;
        }
    }
    fout << "\n" << std::flush;
*/
    events.pop();
    return true;
}

//----------------------------------------//
