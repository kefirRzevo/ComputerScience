#include "../include/view.hpp"
#include "../include/textview.hpp"
#include "../include/guiview.hpp"


//----------------------------------------//

View*
View::Get(const std::string& mode)
{
        static std::unique_ptr<View> view = nullptr;

        if(view)
                return view.get();

        if(!mode.compare("gui"))
                view.reset(new GuiView());
        else if(!mode.compare("text"))
                view.reset(new TextView());

        return view.get();   
}

//----------------------------------------//

void
View::PollOnTimer(int passedTime)
{
    for(size_t i = 0; i < passedTimes.size(); i++)
    {
        passedTimes[i] += passedTime;
        if(passedTimes[i] > listenersOnTimer[i].first)
        {
            passedTimes[i] %= listenersOnTimer[i].first;
            listenersOnTimer[i].second();
        }
    }
}

//----------------------------------------//
