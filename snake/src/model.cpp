#include "../include/model.hpp"

//#include<fstream>
//std::ofstream fout("dump.txt");

//static int step = 0;

//--------------------PROTOTYPES--------------------

bool operator==(const Coordinate& lhs, const Coordinate& rhs);

//static void
//Dump(Snakes& snakes);

static Coordinate
GetRandomPoint(const size& polygonSize);

static bool
Impacts(const Coordinates& coords1, const Coordinates& coords2,
                                                Coordinates* impacts);

static bool
SnakeImpacts(const Snakes& snakes, Coordinates* impacts);

static bool
RabbitImpacts(const Snakes& snakes, const Coordinates& rabbitCoords, Coordinates* impacts);


//--------------------GENERAL--------------------

bool
operator==(const Coordinate& lhs, const Coordinate& rhs)
{
    return lhs.first == rhs.first && lhs.second == rhs.second;
}

static
Coordinate GetRandomPoint(const size& polygonSize)
{
    Coordinate coord = {};
    coord.first  = std::rand() % polygonSize.first + 1;
    coord.second = std::rand() % polygonSize.second + 1;

    return coord;
}


Model::Model(size polySize, int nSnakes)
{
    polygonSize = polySize;
    int nRabbits = 10;
    sizes snakeSizes(nSnakes, 8);
    Randomize(nRabbits, snakeSizes);
};


//--------------------IMPACTS--------------------

static bool
Impacts(const Coordinates& coords1, const Coordinates& coords2,
                                                Coordinates* impacts)
{
    bool impact = false;
    for(const auto& i: coords1)
    {
        for(const auto& j: coords2)
        {
            if(i == j)
            {
                if(impacts)
                    impacts->push_front(i);

                impact = true;
            }
        }
    }

    return impact;
}

static bool
SnakeImpacts(const Snakes& snakes, Coordinates* impacts)
{
    size_t nSnakes = snakes.size();

    if(nSnakes <= 1)
        return false;

    bool impact = false;
    for(size_t i = 0; i < nSnakes; i++)
        for(size_t j = i + 1; j < nSnakes; j++)
            if(Impacts(snakes[i].GetCoordinates(), snakes[j].GetCoordinates(), impacts))
                impact = true;

    return impact;
}

static bool
RabbitImpacts(const Snakes& snakes, const Coordinates& rabbitCoords, 
                                                Coordinates* impacts)
{
    bool impact = false;
    for(const auto& i: snakes)
        if(Impacts(rabbitCoords, i.GetCoordinates(), impacts))
            impact = true;

    return impact;
}


//--------------------SNAKE--------------------

void
Snake::SetDirection(Direction dir)
{
    switch (dir)
    {
        case UP:
            direction = {0, 1};
            break;
        case RIGHT:
            direction = {1, 0};
            break;
        case DOWN:
            direction = {0, -1};
            break;
        case LEFT:
        default:
            direction = {-1, 0};
            break;
    }
}

void
Snake::TurnLeft()
{
    direction = {direction.second, -direction.first};
}

void
Snake::TurnRight()
{
    direction = {-direction.second, direction.first};
}

void
Snake::Generate(size_t snakeSize, const size& polygonSize)
{
    coordinates.clear();

    SetDirection(Direction(std::rand() % 4));
    Coordinate snakeTail = GetRandomPoint(polygonSize);
    Coordinate snakeSection = {};

    snakeSection.first  = snakeTail.first;
    snakeSection.second = snakeTail.second;

    for(size_t j = 0; j < snakeSize; j++)
    {
        if(!direction.first)
            snakeSection.second = snakeTail.second + j * direction.second;
        else
            snakeSection.first = snakeTail.first + j * direction.first;

        coordinates.push_front(snakeSection);
    }
}

void
Snake::Update(const Coordinate& polygonSize)
{
    Coordinate oldHead = coordinates.front();
    Coordinate newHead = {  oldHead.first + direction.first,
                            oldHead.second + direction.second};
    coordinates.pop_back();
    coordinates.push_front(newHead);
}


//--------------------MODEL--------------------

void
Model::Randomize(size_t nRabbits, sizes snakeSizes)
{
    rabbits.clear();
    snakes.clear();

    Snake snake{};
    for(const auto snakeSize: snakeSizes)
    {
        do
        {
            snake.Generate(snakeSize, polygonSize);
        } while (SnakeImpacts(snakes, nullptr));

        snakes.push_back(snake);
    }

    Coordinate  rabbitCoord = {};
    for(size_t i = 0; i < nRabbits; i++)
    {
        do
        {
            rabbitCoord = GetRandomPoint(polygonSize);
        } while (RabbitImpacts(snakes, {rabbitCoord}, nullptr) ||
                 (std::find(rabbits.begin(), rabbits.end(), rabbitCoord) != rabbits.end()));

        rabbits.push_back({rabbitCoord});
    }
}

void
Model::Update()
{
    //Dump(snakes);
    for(auto& snake: snakes)
    {
        snake.Update(polygonSize);
    }
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
