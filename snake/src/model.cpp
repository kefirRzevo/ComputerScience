#include "../include/model.hpp"

bool operator==(const pair& lhs, const pair& rhs)
{
    return lhs.first == rhs.first && lhs.second == rhs.second;
}

static Coordinate GetRandomPoint(const pair& polygonSize)
{
    Coordinate coord = {};
    coord.first  = std::rand() % (polygonSize.first  - 2) + 1;
    coord.second = std::rand() % (polygonSize.second - 2) + 1;
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

void Model::Randomize(int nRabbits, int snakeSize)
{
    rabbits.clear();
    snakes.clear();

    Snake snake = {};
    Coordinate snakeSection = {};
    Coordinates snakeCoords = {};

    Coordinate snakeDirection = GetRandomDirection();
    Coordinate snakeTail = GetRandomPoint(polygonSize);

    for(int i = 0; i < snakeSize; i++)
    {
        if(!snakeDirection.first)
            snakeSection.second = snakeTail.second + i * snakeDirection.second;
        else
            snakeSection.first = snakeTail.first + i * snakeDirection.first;

        snakeCoords.push_front(snakeSection);
    }

    snake.SetDirection(snakeDirection);
    snake.SetCoordinates(snakeCoords);
    snakes.push_back(snake);

    for(int i = 0; i < nRabbits; i++)
    {
        Rabbit rabbit = {};
        bool exists;
        do
        {
            rabbit = GetRandomPoint(polygonSize);
        } while(std::find(rabbits.begin(), rabbits.end(), 
                rabbit) == rabbits.end() &&
                std::find(snakeCoords.begin(), snakeCoords.end(), 
                rabbit.GetCoordinate()) == snakeCoords.end());
                
        rabbits.push_back(rabbit);
    }
}

void Model::Update(const pair& newPolygonSize)
{

}
