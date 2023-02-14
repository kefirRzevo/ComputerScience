#pragma once

#include <iostream>

class View
{
    public:
        View()
            {};

        virtual ~View()
            {};

        virtual void draw() = 0;
};

class GuiView: public View
{
    public:
        void draw();

        GuiView()
            {};

        ~GuiView()
            {};
};

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
