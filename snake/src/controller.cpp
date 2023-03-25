#include "../include/controller.hpp"

#ifdef DEBUG
#include<fstream>
std::ofstream fout1("controller.txt");
#endif


//----------------------------------------//

void 
HumanController::OnKey(int key)
{
    if(!GetSnake()->GetUpdated())
        return;

    Snake* snake = GetSnake();
    Snake::Direction snakeDir = snake->GetDirection();

    if(key == leftKey)
    {
        if( snakeDir == Snake::Direction::LEFT ||
            snakeDir == Snake::Direction::RIGHT )
            goto leave;

        snake->SetDirection(Snake::Direction::LEFT);
    }
    else if(key == rightKey)
    {
        if( snakeDir == Snake::Direction::LEFT ||
            snakeDir == Snake::Direction::RIGHT )
            goto leave;

        snake->SetDirection(Snake::Direction::RIGHT);
    }
    else if(key == upKey)
    {
        if( snakeDir == Snake::Direction::DOWN ||
            snakeDir == Snake::Direction::UP )
            goto leave;

        snake->SetDirection(Snake::Direction::UP);
    }
    else if(key == downKey)
    {
        if( snakeDir == Snake::Direction::DOWN ||
            snakeDir == Snake::Direction::UP )
            goto leave;

        snake->SetDirection(Snake::Direction::DOWN);
    }

leave:

    snake->UnsetUpdated();
}

//----------------------------------------//

void
BotController::OnTimer(int passedTime)
{
    assert(passedTime);

    if(!GetSnake()->GetUpdated())
        return;

    static int prevScore = 0;
    View* view = View::Get();
    Model* model = view->GetModel();

    Snake* snake = GetSnake();
    Snake::Direction snakeDir = snake->GetDirection();

    Coordinate victimCoord = *static_cast<Coordinate* >(victim);
    Coordinate front = snake->GetFront();


    if(snake->GetScore() > prevScore)
    {
        prevScore = snake->GetScore();
        victim = model->GetClosestRabbit(GetSnake()->GetFront());
        victimCoord = *static_cast<Coordinate* >(victim);
    }

    if(front.first < victimCoord.first)
    {
        if(front.second < victimCoord.second && snakeDir == Snake::Direction::LEFT)
            snake->SetDirection(Snake::Direction::DOWN);
        else if(front.second >= victimCoord.second && snakeDir == Snake::Direction::LEFT)
            snake->SetDirection(Snake::Direction::UP);
        else if(snakeDir != Snake::Direction::LEFT)
            snake->SetDirection(Snake::Direction::RIGHT);
        else
            assert(0);
    }
    else if(front.first > victimCoord.first)
    {
        if(front.second <= victimCoord.second && snakeDir == Snake::Direction::RIGHT)
            snake->SetDirection(Snake::Direction::DOWN);
        else if(front.second > victimCoord.second && snakeDir == Snake::Direction::RIGHT)
            snake->SetDirection(Snake::Direction::UP);
        else
            snake->SetDirection(Snake::Direction::LEFT);
    }
    else if(front.second < victimCoord.second)
    {
        if(snakeDir == Snake::Direction::UP)
            snake->SetDirection(Snake::Direction::LEFT);
        else
            snake->SetDirection(Snake::Direction::DOWN);
    }
    else
    {
        if(snakeDir == Snake::Direction::DOWN)
            snake->SetDirection(Snake::Direction::RIGHT);
        else
            snake->SetDirection(Snake::Direction::UP);
    }

    snake->UnsetUpdated();
}

//----------------------------------------//
