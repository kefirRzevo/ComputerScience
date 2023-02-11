#pragma once

#include <iostream>

#include "view.hpp"

class TextView: public View
{
    public:
        void draw()
        {
            std::cout << "Hello world" << std::endl;
        }

        TextView()
            {};

        ~TextView()
            {};
};
