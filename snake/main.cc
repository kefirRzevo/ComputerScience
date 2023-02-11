#include <iostream>

#include "view/view.hpp"
#include "view/guiview.hpp"
#include "view/textview.hpp"

int main(int argc, char* argv[])
{
    if(argc != 2)
        return 0;

    View* gameView = nullptr;
    if(!strcmp(argv[1], "0"))
    {
        gameView = new GuiView();
    }
    else if(!strcmp(argv[1], "1"))
    {
        gameView = new TextView();
    }
    else
    {
        return 1;
    }
    gameView->draw();
    
    return 0;
}
