#pragma once


//#include <SFML/Graphics.hpp>

#include "view.hpp"


//----------------------------------------//

class GuiView: public View
{
    public:
        GuiView()
            {};

        ~GuiView()
            {}

        void RunLoop() override
            {
                std::cout << "Not implemented" << std::endl;
            }

    private:

        void UpdateWindowSize() override
            {
                std::cout << "Not implemented" << std::endl;
            }
};

//----------------------------------------//
