#pragma once


#include <iostream>
#include <cstdlib>
#include <unistd.h>

#include "model.hpp"

using OnKeyCall = std::function<void(int)>;
using OnTimerCall = std::function<void(int)>;

class View
{
    public:

        View() = default;

        virtual ~View()
            {}

	    static View* Get(const std::string& mode = "text");

        virtual void RunLoop() = 0;
    
        Size GetWindowSize() const 
            {
                return windowSize;
            }

        Model* GetModel() const
            {
                return model;
            }

        void SetModel(Model* mod)
            {
                model = mod;
            }

        void SetOnKey(OnKeyCall OnKey)
            {
                listenersOnKey.push_back(OnKey);
            }

        void SetOnTimer(OnTimerCall OnTimer)
            {
                listenersOnTimer.push_back(OnTimer);
            }

    protected:

        Model* model;
        bool finished;
        Size windowSize;

        std::vector<OnKeyCall>   listenersOnKey;
        std::vector<OnTimerCall> listenersOnTimer;

        friend void SigHandler(int signum);

        void PollOnTimer(int msecPassed);

        virtual void UpdateWindowSize() = 0;
};
