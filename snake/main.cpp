#include <iostream>

#include "include/view.hpp"
#include "include/controller.hpp"

int main(int argc, char* argv[])
{std::srand(std::time(nullptr));
    View* view = nullptr;

    if(argc > 1)
        view = View::Get(argv[1]);
    else
        view = View::Get();

    Model model({POLYGON_WIDTH, POLYGON_HEIGHT}, N_SNAKES);
    view->SetModel(&model);
    BotController(model.GetSnake(0));
    BotController(model.GetSnake(1));
    BotController(model.GetSnake(2));
    //HumanController(model.GetSnake(1), keyArrowL, keyArrowR, keyArrowU, keyArrowD);
    //HumanController(model.GetSnake(2), keyA, keyD, keyW, keyS);

    view->RunLoop();

    return 0;
}
