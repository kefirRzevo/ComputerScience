#include <iostream>

#include "include/view.hpp"

int main(int argc, char* argv[])
{
        View* view = nullptr;
        if(argc > 1)
                view = View::Get(argv[1]);
        else
                view =View::Get();

        while(1)
        {
                view->Draw();
        }
    
        return 0;
}
