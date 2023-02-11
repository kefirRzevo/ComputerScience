#pragma once

#include <iostream>

#include "view.hpp"

class GuiView: public View
{
    public:
        void draw();

        GuiView()
            {};

        ~GuiView()
            {};
};
