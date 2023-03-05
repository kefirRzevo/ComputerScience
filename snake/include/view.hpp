#pragma once

#include <iostream>
#include <cstdlib>
#include <unistd.h>

#include "model.hpp"

using size = std::pair<size_t, size_t>;


class View
{
    public:

        View() = default;

        virtual ~View()
            {}

	    static View* Get(const std::string& mode = "text");

        virtual void Run() = 0;
        virtual size GetWindowSize() const = 0;
        virtual bool GetStatus() const = 0;

        Model* GetModel() const
            {
                return model;
            }

        void SetModel(Model* mod)
            {
                model = mod;
            }

        void SetOnKey(std::function<void(int)> onKey)
            {
                listenersOnKey.push_back(onKey);
            }

    protected:

        Model* model;
        std::vector<std::function<void(int)>> listenersOnKey;

        virtual void UpdateWindowSize() = 0;
        virtual void UpdateStatus() = 0;

        friend void SigHandler(int signum);
};
