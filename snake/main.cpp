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

    Model model({50, 20}, 2);
    view->SetModel(&model);
    HumanController(model.GetSnake(0), 'a', 'd', 'w', 's');
    BotController(model.GetSnake(1));
    //HumanController(model.GetSnake(2), 'l', 'r', 'u', 'n');

    view->RunLoop();

    return 0;
}
