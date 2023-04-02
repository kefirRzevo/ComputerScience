#pragma once


#include <iostream>
#include <cstdlib>
#include <unistd.h>

#include "model.hpp"

using OnKeyCall = std::function<void(int)>;
using OnTimerCall = std::pair<int, std::function<void(void)>>;

class View
{
    public:

        View() = default;

        virtual ~View()
            {}

	    static View* Get(const std::string& mode = "gui");

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
                passedTimes.push_back(OnTimer.first);
            }

    protected:

        Model* model;
        Size windowSize;
        bool finished;

        std::vector<OnKeyCall> listenersOnKey;
        std::vector<OnTimerCall> listenersOnTimer;
        std::vector<int> passedTimes;

        friend void SigHandler(int signum);

        void PollOnKey(int key);

        void PollOnTimer(int microsecPassed);

        virtual void UpdateWindowSize() = 0;
};
