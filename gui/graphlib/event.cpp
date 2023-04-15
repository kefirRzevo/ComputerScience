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
       type == mouseHovered  || type == mouseMoved )
        return true;

    return false;
}

//----------------------------------------//

EventManager*
EventManager::Get()
{
    static std::unique_ptr<EventManager> eventManager;

    if(eventManager.get() == nullptr)
        eventManager.reset(new EventManager());

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

    Vec2i lastPos = {};
    sf::Event sfEvent;

    if(!Renderer::Get()->Get_SF_RenderWindow()->pollEvent(sfEvent))
        return false;

    switch (sfEvent.type)
    {
        case sf::Event::Closed:

            events.push({quit});
            return true;

        case sf::Event::KeyPressed:

            events.push({keyPressed, {sfEvent.key.code}});
            return true;

        case sf::Event::TextEntered:
            events.push({textEntered, {static_cast<int>(sfEvent.text.unicode)}});
            return true;

        case sf::Event::MouseButtonPressed:pressed = true;
            lastPos = {sfEvent.mouseButton.x, sfEvent.mouseButton.y};
            prevPos = lastPos;
            if(sfEvent.mouseButton.button != sf::Mouse::Left)
                break;
            events.push({mousePressed, {lastPos}});
            return true;

        case sf::Event::MouseButtonReleased:

            pressed = false;
            lastPos = {sfEvent.mouseButton.x, sfEvent.mouseButton.y};
            if(sfEvent.mouseButton.button != sf::Mouse::Left)
                break;
            events.push({mouseReleased, {lastPos}});
            events.push({mouseHovered,  {lastPos}});
            return true;

        case sf::Event::MouseMoved:

            lastPos = {sfEvent.mouseMove.x, sfEvent.mouseMove.y};
            if(pressed)
            {
                events.push({mouseMoved, {prevPos, lastPos - prevPos}});
                prevPos = lastPos;
            }
            events.push({mouseHovered, {lastPos}});
            return true;

        default:
            break;
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
        case Event::keyPressed:
            fout << "keyPressed ";
            fout << event.key.key;
            break;
        
        case Event::mousePressed:
            fout << "mousePressed ";
            fout << event.mouse.pos.x << " " << event.mouse.pos.y;
            break;

        case Event::mouseReleased:
            fout << "mouseReleased ";
            fout << event.mouse.pos.x << " " << event.mouse.pos.y;
            break;

        case Event::mouseMoved:
            fout << "mouseMoved ";
            fout << event.mouse.pos.x << " " << event.mouse.pos.y << " & " << event.mouse.offset.x << " " << event.mouse.offset.y;
            break;

        case Event::mouseHovered:
            fout << "mouseHovered ";
            fout << event.mouse.pos.x << " " << event.mouse.pos.y;
            break;
        
        default:
            fout << "WTF";
            break;
    }
    fout << "\n" << std::flush;
*/
    events.pop();
    return true;
}

//----------------------------------------//
