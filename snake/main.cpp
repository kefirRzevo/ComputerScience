#include <iostream>

#include "include/view.hpp"
#include "include/controller.hpp"

int main(int argc, char* argv[])
{
        View* view = nullptr;

        if(argc > 1)
                view = View::Get(argv[1]);
        else
                view = View::Get();

        Model model({50, 50}, 1);
        view->SetModel(&model);
        HumanController(model.GetSnake(0));

        view->Run();

        return 0;
}
