#include "../include/model.hpp"
#include "../include/view.hpp"

#ifdef DEBUG
#include<fstream>
std::ofstream fout2("model.txt");
#endif

static Coordinate
GetRandomPoint(const Size& polygonSize);

static int
GetDistance(const Coordinate* coord1, const Coordinate* coord2);

static bool
GoingTowards(const Snake* snake1, const Snake* snake2);


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

static int
GetDistance(const Coordinate* coord1, const Coordinate* coord2)
{
     return abs(coord1->first  - coord2->first) +
            abs(coord1->second - coord2->second) - 1;
}

//----------------------------------------//

static bool
GoingTowards(const Snake* snake1, const Snake* snake2)
{
    Snake::Direction dir1 = snake1->GetDirection();
    Snake::Direction dir2 = snake2->GetDirection();

    if( (dir1 == Snake::Direction::DOWN  && dir2 == Snake::Direction::UP)    ||
        (dir1 == Snake::Direction::UP    && dir2 == Snake::Direction::DOWN)  ||
        (dir1 == Snake::Direction::LEFT  && dir2 == Snake::Direction::RIGHT) ||
        (dir1 == Snake::Direction::RIGHT && dir2 == Snake::Direction::LEFT) )
        return true;
    
    return false;
}

//----------------------------------------//

bool
Rabbit::RandomGenerate(Model* model, size_t score_)
{
    assert(model);

    size_t counter = 0;
    Coordinate coord = {};

initializeRabbit:

    counter++;
    if(counter > 50)
        return false;

    coord = GetRandomPoint(model->GetPolygonSize());

    for(const auto& rabbit: model->rabbits)
    {
        if(rabbit == this)
            continue;

        if(coord == rabbit->GetCoordinate())
            goto initializeRabbit;
    }

    for(const auto& snake: model->snakes)
        for(const auto& section: snake->GetCoordinates())
            if(coord == section)
                goto initializeRabbit;

    SetCoordinate(coord);
    score = score_;
    Hill();
    
    return true;
}

//----------------------------------------//

bool
Snake::RandomGenerate(Model* model, size_t snakeSize)
{
    assert(model);

    int iterator = 0;
    int counter = 0;

    Coordinate direction = {};
    Coordinate back = {};
    Coordinate section = {};

initializeSnake:

    counter++;
    if(counter > 50)
        return false;

    coords.clear();

    SetDirection(Direction(std::rand() % 4));

    direction = GetCoordinateDirection();
    back      = GetRandomPoint(model->GetPolygonSize());

    section.first  = back.first;
    section.second = back.second;

    for(iterator = 0; iterator < static_cast<int>(snakeSize); iterator++)
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
            if(snake == this)
                continue;

            for(const auto& snakeSection: snake->GetCoordinates())
                if(snakeSection == section)
                    goto initializeSnake;
        }

        coords.push_front(section);
    }

    return true;
}

//----------------------------------------//

Coordinate
Snake::GetCoordinateDirection() const
{
    switch(dir)
    {
        case Direction::UP:
            return {0, -1};
        
        case Direction::RIGHT:
            return {1, 0};

        case Direction::LEFT:
            return {-1, 0};

        case Direction::DOWN:

        default:
            return {0, 1};
    }

    return {0, 1};
}

//----------------------------------------//

void
Snake::Update(Model* model)
{
    assert(model);

    updated = true;
    if(!alive)
        return;

    Coordinate direction = GetCoordinateDirection();
    Coordinate newFront  = {coords.front().first  + direction.first,
                            coords.front().second + direction.second};

    for(const auto& snake: model->snakes)
        for(const auto& section: snake->GetCoordinates())
            if(section == newFront)
            {
                alive = false;
                if(newFront == snake->GetFront() && GoingTowards(this, snake))
                    snake->Kill();
                return;
            }

    coords.push_front(newFront);

    for(auto& rabbit: model->rabbits)
        if(static_cast<Coordinate>(*rabbit) == newFront)
        {
            Feed(rabbit);
            rabbit->Kill();
        }

    if(scoreReserved == 0)
        coords.pop_back();
    else
        scoreReserved--;
}

//--------------------MODEL--------------------

Model::Model(Size polygonSize_, size_t nSnakes)
{
    polygonSize = polygonSize_;
    finished = false;
    
    for(size_t i = 0; i < nSnakes; i++)
    {
        Snake* snake = new Snake();
        if(!snake->RandomGenerate(this))
            assert(0);
        snakes.push_back(snake);
    }

    for(size_t i = 0; i < 10; i++)
    {
        Rabbit* rabbit = new Rabbit();
        if(!rabbit->RandomGenerate(this))
            assert(0);
        rabbits.push_front(rabbit);
    }

    View* view = View::Get();
    view->SetOnTimer(std::bind(&Model::OnTimer, this, std::placeholders::_1));
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
    for(size_t i = 0; i < snakes.size(); i++)
    {
        snakes[i]->Update(this);
        if(!snakes[i]->IsAlive())
        {
            snakes[i]->Kill();
            finished = true;
        }
    }

    for(const auto& rabbit: rabbits)
        if(rabbit->IsKilled())
            rabbit->RandomGenerate(this);
}

//----------------------------------------//

void
Model::OnTimer(int passedTime)
{
    static int sumPassedTime = 0;
    sumPassedTime += passedTime;

    if(sumPassedTime < 500)
        return;

    sumPassedTime = sumPassedTime % 500;

    Update();
}

//----------------------------------------//

Rabbit*
Model::GetClosestRabbit(const Coordinate& point)
{
    Rabbit* closestRabbit = *rabbits.begin();
    int closestDistance = GetDistance(static_cast<Coordinate* >(closestRabbit),
                                      &point);

    int distance = 0;
    for(const auto& rabbit: rabbits)
    {
        distance = GetDistance(static_cast<Coordinate* >(rabbit), &point);
        if(distance < closestDistance)
        {
            closestRabbit = rabbit;
            closestDistance = distance;
        }
    }

    return closestRabbit;
}

//----------------------------------------//
