#include "../include/model.hpp"

#include<fstream>

std::ofstream fout1("dump.txt");
int step = 0;

bool operator==(const Coordinate& lhs, const Coordinate& rhs);

static void Dump(Snakes& snakes);

static Coordinate GetRandomPoint(const size& polygonSize);

static Coordinate GetRandomDirection();

static bool Impacts(const Coordinates& coords1, const Coordinates& coords2,
                                                Coordinates* impacts);

static bool SnakeImpacts(const Snakes& snakes, Coordinates* impacts);

static bool RabbitImpacts(const Snakes& snakes, const Coordinates& rabbitCoords, 
                                                Coordinates* impacts);

static void GenerateSnake(Snake& snake, size_t snakeSize, 
                                        const size& polygonSize);


bool operator==(const Coordinate& lhs, const Coordinate& rhs)
{
    return lhs.first == rhs.first && lhs.second == rhs.second;
}

static Coordinate GetRandomPoint(const size& polygonSize)
{
    Coordinate coord = {};
    coord.first  = std::rand() % polygonSize.first + 1;
    coord.second = std::rand() % polygonSize.second + 1;

    return coord;
}

static Coordinate GetRandomDirection()
{
    Coordinate coord = {};
    int x = 2 * (std::rand() % 2) - 1; // 1; -1
    int y = std::rand() % 2;           // 0;  1
    coord.first  = x*y;
    coord.second = x-x*y;

    return coord;
}

static bool Impacts(const Coordinates& coords1, const Coordinates& coords2,
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

static bool SnakeImpacts(const Snakes& snakes, Coordinates* impacts)
{
    size_t nSnakes = snakes.size();

    if(nSnakes <= 1)
        return false;

    bool impact = false;
    for(size_t i = 0; i < nSnakes; i++)
        for(size_t j = i + 1; j < nSnakes; j++)
            if(Impacts(snakes[i].SeeCoordinates(), snakes[j].SeeCoordinates(), impacts))
                impact = true;

    return impact;
}

static bool RabbitImpacts(const Snakes& snakes, const Coordinates& rabbitCoords, 
                                                Coordinates* impacts)
{
    bool impact = false;
    for(const auto& i: snakes)
        if(Impacts(rabbitCoords, i.SeeCoordinates(), impacts))
            impact = true;

    return impact;
}

static void GenerateSnake(Snake& snake, size_t snakeSize, 
                                        const size& polygonSize)
{
    Coordinate  snakeSection = {};
    Coordinates snakeSections = {};

    Coordinate snakeDirection = GetRandomDirection();
    Coordinate snakeTail = GetRandomPoint(polygonSize);

    snakeSection.first  = snakeTail.first;
    snakeSection.second = snakeTail.second;
    
    for(size_t j = 0; j < snakeSize; j++)
    {
        if(!snakeDirection.first)
            snakeSection.second = snakeTail.second + j * snakeDirection.second;
        else
            snakeSection.first = snakeTail.first + j * snakeDirection.first;

        snakeSections.push_front(snakeSection);
    }

    snake.GetDirection() = snakeDirection;
    snake.GetCoordinates() = snakeSections;
}

void Model::Randomize(size_t nRabbits, sizes snakeSizes)
{
    rabbits.clear();
    snakes.clear();

    Snake snake{};
    for(const auto snakeSize: snakeSizes)
    {
        do
        {
            GenerateSnake(snake, snakeSize, polygonSize);
        } while (SnakeImpacts(snakes, nullptr));

        snakes.push_back(snake);
    }

    Coordinates rabbitsCoords = {};
    Coordinate  rabbitCoord = {};
    for(size_t i = 0; i < nRabbits; i++)
    {
        do
        {
            rabbitsCoords.clear();
            rabbitCoord = GetRandomPoint(polygonSize);
            rabbitsCoords.push_front(rabbitCoord);
        } while (RabbitImpacts(snakes, rabbitsCoords, nullptr) ||
                 (std::find(rabbits.begin(), rabbits.end(), rabbitCoord) != rabbits.end()));

        rabbits.push_back({rabbitCoord});
    }
}

void Model::Update()
{
    //Dump(snakes);
    for(auto& snake: snakes)
    {
        Coordinates& snakeCoords = snake.GetCoordinates();
        Coordinate oldHead = snakeCoords.front();
        Coordinate newHead = {oldHead.first  + snake.SeeDirection().first,
                              oldHead.second + snake.SeeDirection().second};
        snakeCoords.pop_back();
        snakeCoords.push_front(newHead);
    }
}

static void Dump(Snakes& snakes)
{
    int i = 0;
    fout1 << "iterator " << step++ << std::endl;
    for(const auto& snake: snakes)
    {
        fout1 << "Snake: " << i << std::endl;

        for(const auto& j: snake.SeeCoordinates())
        {
            fout1 << j.first << " " << j.second << ";\t";
        }
        fout1 << std::endl;
        i++;
    }
}
