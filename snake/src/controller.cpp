#include "../include/controller.hpp"

//#include <fstream>
//std::ofstream fout("dump.txt");


void 
HumanController::OnKey(int key)
{
    Snake* snake = GetSnake();

    switch (key)
    {
        case 'a':
            snake->TurnLeft();
            break;
        
        case 'd':
            snake->TurnRight();
            break;

        default:
            break;
    }
}
