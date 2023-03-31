#include "../include/controller.hpp"

#define DEBUG

#ifdef DEBUG
#include<fstream>
std::ofstream fout1("controller.txt");
#endif


static Coordinate
GetShift(const Coordinate& coord, Snake::Direction dir);


//----------------------------------------//

static Coordinate
GetShift(const Coordinate& coord, Snake::Direction dir)
{
    Coordinate dirCoord = Snake::GetCoordDirection(dir);
    return {coord.first + dirCoord.first, coord.second + dirCoord.second};
}

//----------------------------------------//

void 
HumanController::OnKey(int key)
{
    if(!snake->IsAlive())
        return;

    if(key == leftKey)
        snake->SetDirection(Snake::LEFT);
    else if(key == rightKey)
        snake->SetDirection(Snake::RIGHT);
    else if(key == upKey)
        snake->SetDirection(Snake::UP);
    else if(key == downKey)
        snake->SetDirection(Snake::DOWN);
}

//----------------------------------------//

bool
BotController::IsPointSecure(const Coordinate& point) const
{
    Model* model = View::Get()->GetModel();
    for(const auto& snake_: model->snakes)
    {
        for(const auto& section: snake_->GetCoordinates())
            if(section == point)
                return false;
        
        if(snake_ == snake)
            continue;

        Coordinate newFront = GetShift(snake_->GetFront(), snake_->GetDirection());
        if(newFront == point)
            return false;
    }

    return true;
}

//----------------------------------------//

void
BotController::GetDirsPriority()
{
    Snake::Direction prevDir = snake->GetDirection();
    Coordinate prevFront = snake->GetFront();

    for(size_t i = 0; i < 4; i++)
    {
        Snake::Direction newDir = dirPriors[i].second;
        Coordinate newFront = GetShift(prevFront, newDir);

        if(!IsPointSecure(newFront))
        {
            dirPriors[i].first = 0;
            continue;
        }

        int prevDistance = Model::GetDistance(target, prevFront);
        int newDistance  = Model::GetDistance(target, newFront);

        if(newDistance < prevDistance && prevDir == newDir)
        {
            dirPriors[i].first = 8;
            continue;
        }

        if(newDistance < prevDistance)
        {
            dirPriors[i].first = 4;
            continue;
        }

        if(prevDir != newDir)
        {
            dirPriors[i].first = 2;
            continue;
        }

        dirPriors[i].first = 1;
    }
}

//----------------------------------------//

void
BotController::OnTimer()
{
    if(!snake->IsAlive())
        return;

    Model* model = View::Get()->GetModel();
    Coordinate front = snake->GetFront();

    if(!model->IsThereRabbit(target))
        target = model->GetClosestRabbitCoord(front);

    GetDirsPriority();

    DirPriority highestPrior = dirPriors[0];
    for(size_t i = 1; i < 4; i++)
        if(dirPriors[i].first > highestPrior.first)
            highestPrior = dirPriors[i];

    snake->SetDirection(highestPrior.second);
    snake->Dump();
}

//----------------------------------------//
