#pragma once


#include <iostream>
#include <cstdlib>
#include <unistd.h>

#include "model.hpp"


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

        void SetOnKey(std::function<void(int)> OnKey)
            {
                listenersOnKey.push_back(OnKey);
            }

        void SetOnTimer(std::function<void(int)> OnTimer)
            {
                listenersOnTimer.push_back(OnTimer);
            }

    protected:

        Model* model;
        bool finished;
        Size windowSize;

        std::vector<std::function<void(int)>> listenersOnKey;
        std::vector<std::function<void(int)>> listenersOnTimer;

        friend void SigHandler(int signum);

        virtual void UpdateWindowSize() = 0;
};
