#include "../include/controller.hpp"

//#include <fstream>
//std::ofstream fout("dump.txt");


void 
HumanController::OnKey(int key)
{
    Snake* snake = GetSnake();
    Snake::Direction snakeDir = snake->GetDirection();

    if(key == leftKey)
    {
        if( snakeDir == Snake::Direction::LEFT ||
            snakeDir == Snake::Direction::RIGHT )
            return;

        snake->SetDirection(Snake::Direction::LEFT);
        return;
    }
    else if(key == rightKey)
    {
        if( snakeDir == Snake::Direction::LEFT ||
            snakeDir == Snake::Direction::RIGHT )
            return;

        snake->SetDirection(Snake::Direction::RIGHT);
        return;
    }
    else if(key == upKey)
    {
        if( snakeDir == Snake::Direction::DOWN ||
            snakeDir == Snake::Direction::UP )
            return;

        snake->SetDirection(Snake::Direction::DOWN);
        return;
    }
    else if(key == downKey)
    {
        if( snakeDir == Snake::Direction::DOWN ||
            snakeDir == Snake::Direction::UP )
            return;

        snake->SetDirection(Snake::Direction::UP);
        return;
    }
}


