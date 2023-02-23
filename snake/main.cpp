#include <iostream>

#include "include/view.hpp"

int main(int argc, char* argv[])
{
        View* view = nullptr;
        
        if(argc > 1)
                view = View::Get(argv[1]);
        else
                view = View::Get();

        view->Draw();
        while(1)
        {
                view->Redraw();
                usleep(100000);
                if(!view->GetStatus())
                        break;
        }

        delete view;
        return 0;
}
