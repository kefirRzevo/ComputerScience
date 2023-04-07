#include "../include/model.hpp"
#include "../include/view.hpp"


static Coordinate
GetRandomPoint(const Size& polygonSize);


//----------------------------------------//

static Coordinate
GetRandomPoint(const Size& polygonSize)
{
    Coordinate coord = {};
    coord.first  = std::rand() % polygonSize.first  + 1;
    coord.second = std::rand() % polygonSize.second + 1;

    return coord;
}

//----------------------------------------//

int
Model::GetDistance(const Coordinate& coord1, const Coordinate& coord2)
{
    return abs(coord1.first  - coord2.first) +
           abs(coord1.second - coord2.second);
}

//----------------------------------------//

void
Rabbit::RandomGenerate(Model* model, int score_)
{
    assert(model);

    size_t counter = 0;
    Coordinate coord = {};

initializeRabbit:

    counter++;
    if(counter > N_MAX_TRIES)
        assert(0);

    coord = GetRandomPoint(model->GetPolygonSize());

    for(const auto& rabbit: model->rabbits)
    {
        if(rabbit == this)
            continue;

        if(coord == rabbit->GetCoordinate())
            goto initializeRabbit;
    }

    for(const auto& snake: model->snakes)
    {
        if(!snake->IsInGame())
            continue;

        for(const auto& section: snake->GetCoordinates())
            if(coord == section)
                goto initializeRabbit;
    }

    SetCoordinate(coord);
    score = score_;
    Hill();
}

//----------------------------------------//

Snake::Snake(int styleType_):
coords({}), newFront({}), dirInFuture(Direction::UP),
dirAfterUpdate(Direction::UP), styleType(styleType_), scoreReserved(0), score(0),
alive(true), inGame(true)
{
    View* view = View::Get();
    view->SetOnTimer({AFTER_DEATH_MSEC * 1000,
                      std::bind(&Snake::OnTimerDeath, this)});
}

//----------------------------------------//

void
Snake::RandomGenerate(Model* model, int snakeSize)
{
    int iterator = 0;
    int counter = 0;

    Coordinate direction = {};
    Coordinate back = {};
    Coordinate section = {};

initializeSnake:

    counter++;
    if(counter > N_MAX_TRIES)
        assert(0);

    coords.clear();

    dirInFuture = Direction(std::rand() % 4);

    direction = Snake::GetCoordDirection(dirInFuture);
    back      = GetRandomPoint(model->GetPolygonSize());

    section.first  = back.first;
    section.second = back.second;

    for(iterator = 0; iterator < snakeSize; iterator++)
    {
        if(!direction.first)
            section.second = back.second + iterator * direction.second;
        else
            section.first  = back.first  + iterator * direction.first;

        for(const auto& rabbit: model->rabbits)
            if(rabbit->GetCoordinate() == section)
                goto initializeSnake;

        for(const auto& snake: model->snakes)
        {
            if(!snake->IsInGame())
                continue;

            if(snake == this)
                continue;

            for(const auto& snakeSection: snake->GetCoordinates())
                if(snakeSection == section)
                    goto initializeSnake;
        }

        coords.push_front(section);
    }
    dirAfterUpdate = dirInFuture;
}

//----------------------------------------//

Coordinate
Snake::GetCoordDirection(Snake::Direction dir)
{
    switch(dir)
    {
        case Direction::DOWN:
            return {0, 1};
        
        case Direction::RIGHT:
            return {1, 0};

        case Direction::LEFT:
            return {-1, 0};

        case Direction::UP:

        default:
            return {0, -1};
    }

    return {0, -1};
}

//----------------------------------------//

Snake::Direction
Snake::GetOppositeDir(Snake::Direction dir)
{
    switch(dir)
    {
        case Snake::Direction::DOWN:
            return Snake::Direction::UP;
        
        case Snake::Direction::RIGHT:
            return Snake::Direction::LEFT;

        case Snake::Direction::LEFT:
            return Snake::Direction::RIGHT;

        case Snake::Direction::UP:
            return Snake::Direction::DOWN;

        default:
            return dir;
    }

    return dir;
}

//----------------------------------------//

void
Snake::SetNewFront()
{
    if(!inGame)
        return;

    Coordinate direction = Snake::GetCoordDirection(dirInFuture);
    newFront = {coords.front().first  + direction.first,
                coords.front().second + direction.second};
}

//----------------------------------------//

void
Snake::Update(Model* model)
{
    if(!alive)
        return;

    for(const auto& snake: model->snakes)
    {
        if(!snake->IsInGame())
            continue;

        if(snake->GetNewFront() == newFront && snake != this)
        {
            alive = false;
            return;
        }

        for(const auto& section: snake->GetCoordinates())
            if(section == newFront)
            {
                alive = false;
                return;
            }
    }

    coords.push_front(newFront);

    for(auto& rabbit: model->rabbits)
        if(static_cast<Coordinate>(*rabbit) == newFront)
        {
            Feed(rabbit->GetScore());
            rabbit->Kill();
        }

    if(scoreReserved == 0)
        coords.pop_back();
    else
        scoreReserved--;

    UpdateDirection();
}

//----------------------------------------//

void
Snake::OnTimerDeath()
{
    static bool wasCalled = false;

    if(!alive && wasCalled)
        inGame = false;

    if(!alive)
        wasCalled = true;
}

//--------------------MODEL--------------------

Model::Model(Size polygonSize_, size_t nSnakes)
{
    polygonSize = polygonSize_;
    snakesAlive = true;

    for(size_t i = 0; i < nSnakes; i++)
    {
        Snake* snake = new Snake(i);
        snake->RandomGenerate(this);
        snakes.push_back(snake);
    }

    for(size_t i = 0; i < N_RABBITS; i++)
    {
        Rabbit* rabbit = new Rabbit(i);
        rabbit->RandomGenerate(this);
        rabbits.push_front(rabbit);
    }

    View* view = View::Get();
    view->SetOnTimer({TICK_MSEC * 1000, 
                      std::bind(&Model::OnTimer, this)});
}

//----------------------------------------//

Model::~Model()
{
    for(const auto& snake: snakes)
        delete snake;

    for(const auto& rabbit: rabbits)
        delete rabbit;
}

//----------------------------------------//

void
Model::Update()
{
    if(snakes.size() == 0)
        return;

    for(auto& snake: snakes)
        snake->SetNewFront();

    snakesAlive = false;
    for(auto& snake: snakes)
    {
        snake->Update(this);
        if(snake->IsAlive())
            snakesAlive = true;
    }

    for(const auto& rabbit: rabbits)
        if(!rabbit->IsAlive())
            rabbit->RandomGenerate(this);

}

//----------------------------------------//

void
Model::OnTimer()
{
    Update();
}

//----------------------------------------//

Coordinate
Model::GetClosestRabbitCoord(const Coordinate& point) const
{
    Rabbit* closestRabbit = *rabbits.begin();
    int closestDistance = GetDistance(static_cast<Coordinate>(*closestRabbit),
                                      point);

    int distance = 0;
    for(const auto& rabbit: rabbits)
    {
        distance = GetDistance(static_cast<Coordinate>(*rabbit), point);
        if(distance < closestDistance)
        {
            closestRabbit = rabbit;
            closestDistance = distance;
        }
    }

    return *static_cast<Coordinate* >(closestRabbit);
}

//----------------------------------------//

int
Model::GetSnakeIndex(const Snake* snake) const
{
    int index = 0;
    for(auto snakeIterator  = snakes.begin();
             snakeIterator != snakes.end();)
        {
            if(*snakeIterator == snake)
                return index;
            index++;
            snakeIterator++;
        }

    return -1;
}

//----------------------------------------//

void
Snake::Dump() const
{
    assert(fp);
    Model* model = View::Get()->GetModel();

    fprintf(fp, "Index %d; ", model->GetSnakeIndex(this));
    fprintf(fp, "Front %-2d %-2d; ", GetFront().first, GetFront().second);
    fprintf(fp, "NewFront %-2d %-2d; ", GetNewFront().first, GetNewFront().second);
    fprintf(fp, "Alive %d; ", alive);
    fprintf(fp, "Score %-2d; ", this->score);
    fprintf(fp, "\nCoords ");

    for(const auto& coord: coords)
        fprintf(fp, "|%-2d %-2d", coord.first, coord.second);

    fprintf(fp, "|\ndir ");
    switch(dirAfterUpdate)
    {
        case Snake::DOWN:
            fprintf(fp, " Down");
            break;
    
        case Snake::LEFT:
            fprintf(fp, " Left");
            break;

        case Snake::RIGHT:
            fprintf(fp, "Right");
            break;

        default:
            fprintf(fp, "   Up");
    }

    fprintf(fp, "; DirFuture ");

    switch(dirInFuture)
    {
        case Snake::DOWN:
            fprintf(fp, " DOWN");
            break;

        case Snake::LEFT:
            fprintf(fp, " LEFT");
            break;

        case Snake::RIGHT:
            fprintf(fp, "RIGHT");
            break;

        default:
            fprintf(fp, "   UP");
    }
    fprintf(fp, ";\n\n");
    fflush(stdout);
}

//----------------------------------------//

bool
Model::IsThereRabbit(const Coordinate& point) const
{
    for(const auto& rabbit: rabbits)
        if(*static_cast<Coordinate* >(rabbit) == point)
            return true;

    return false;
}

//----------------------------------------//
