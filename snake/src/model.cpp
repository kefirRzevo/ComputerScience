#include "../include/model.hpp"
#include "../include/view.hpp"

//#include<fstream>
//std::ofstream fout("dump.txt");

//static int step = 0;

//--------------------PROTOTYPES--------------------


//static void
//Dump(Snakes& snakes);

static Coordinate
GetRandomPoint(const Size& polygonSize);



//--------------------GENERAL--------------------

static Coordinate
GetRandomPoint(const Size& polygonSize)
{
    Coordinate coord = {};
    coord.first  = std::rand() % polygonSize.first  + 1;
    coord.second = std::rand() % polygonSize.second + 1;

    return coord;
}


//--------------------RABBIT--------------------

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


//--------------------SNAKE--------------------

bool
Snake::RandomGenerate(Model* model, size_t snakeSize)
{
    assert(model);

    size_t iterator = 0;
    size_t counter = 0;

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

Coordinate
Snake::GetCoordinateDirection() const
{
    switch(dir)
    {
        case Direction::UP:
            return {0, 1};
        
        case Direction::RIGHT:
            return {1, 0};

        case Direction::LEFT:
            return {-1, 0};

        case Direction::DOWN:

        default:
            return {0, -1};
    }

    return {0, -1};
}


bool
Snake::Update(Model* model)
{
    assert(model);

    Coordinate direction = GetCoordinateDirection();
    Coordinate newFront  = {coords.front().first  + direction.first,
                            coords.front().second + direction.second};

    for(const auto& snake: model->snakes)
        for(const auto& section: snake->GetCoordinates())
            if(section == newFront)
                return true;

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

    return false;
}


//--------------------MODEL--------------------

Model::Model(Size polygonSize_, GameMode mode_)
{
    polygonSize = polygonSize_;
    mode = mode_;
    finished = false;
    
    Snake* playerSnake = new Snake();
    if(!playerSnake->RandomGenerate(this))
        assert(0);
    snakes.push_back(playerSnake);

    if(mode == GameMode::MULTI)
    {
        Snake* playerSnake2 = new Snake();
        if(!playerSnake2->RandomGenerate(this))
            assert(0);
        snakes.push_back(playerSnake2);
    }

    for(size_t i = 0; i < 10; i++)
    {
        Rabbit* rabbit = new Rabbit();
        if(!rabbit->RandomGenerate(this))
            assert(0);
        rabbits.push_front(rabbit);
    }

    View* view =View::Get(); assert(view);
    view->SetOnTimer(std::bind(&Model::OnTimer, this, std::placeholders::_1));
}


Model::~Model()
{
    for(const auto& snake: snakes)
        delete snake;

    for(const auto& rabbit: rabbits)
        delete rabbit;
}


void
Model::Update()
{
    for(const auto& snake: snakes)
        if(snake->Update(this))
            finished = false;
        
    for(const auto& rabbit: rabbits)
        if(rabbit->IsKilled())
            rabbit->RandomGenerate(this);
}


void
Model::OnTimer(int passedTime)
{
    Update();
}


//--------------------FOR DEBUG--------------------

/*
static void
Dump(Snakes& snakes)
{
    int i = 0;
    fout << "iterator " << step++ << std::endl;
    for(const auto& snake: snakes)
    {
        fout << "Snake: " << i << std::endl;

        for(const auto& j: snake.GetCoordinates())
        {
            fout << j.first << " " << j.second << ";\t";
        }
        fout << std::endl;
        i++;
    }
}
*/

//----------------------------------------//
