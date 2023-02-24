#include <iostream>

#include "include/view.hpp"

int main(int argc, char* argv[])
{
        View* view = nullptr;
        
        if(argc > 1)
                view = View::Get(argv[1]);
        else
                view = View::Get();

        Model model({50, 50});
        view->SetModel(&model);

        while(1)
        {
                view->Draw();
                usleep(500000);
                if(!view->GetStatus())
                        break;
        }

        delete view;
        return 0;
}
