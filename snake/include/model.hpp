#pragma once

#include <iostream>
#include <vector>
#include <list>

using pair        = std::pair<int, int>;
using Coordinate  = pair;
using Coordinates = std::list<Coordinate>;

class Rabbit
{
    private:
        Coordinate coordinate;

    public:
        Rabbit(const Coordinate& coord):
            coordinate(coord) {}

        Rabbit(const Rabbit& rab) = default;

        Rabbit() = default;

        Rabbit& operator=(const Coordinate& coord)
            {
                coordinate = coord;
                return *this;
            }

        bool operator==(const Rabbit& rhs)
            {
                return coordinate == rhs.coordinate;
            }

        ~Rabbit()
            {};

        Coordinate GetCoordinate() const
            {
                return coordinate;
            }

        void SetCoordinate(const Coordinate& coord)
            {
                coordinate = coord;
            }
};

class Snake
{
    private:
        Coordinates coordinates;
        Coordinate  direction;

    public:
        Snake(const Coordinates& coords, const Coordinate& dir):
            coordinates(coords), direction(dir) {}

        Snake(const Snake& snake) = default;

        Snake() = default;

        ~Snake()
            {};

        Coordinates GetCoordinates() const
            {
                return coordinates;
            }

        Coordinate GetDirection() const
            {
                return direction;
            }

        void SetCoordinates(const Coordinates& coords)
            {
                coordinates = coords;
            }

        void SetDirection(const Coordinate& dir)
            {
                direction = dir;
            }

        void PushSection(const Coordinate& coord)
            {
                coordinates.push_front(coord);
            }

        void PopSection()
            {
                coordinates.pop_back();
            }
};

using Rabbits = std::vector<Rabbit>;
using Snakes  = std::vector<Snake>;

class Model
{
    private:
        Rabbits rabbits;
        Snakes  snakes;
        pair polygonSize;

    public:
        Model() = delete;

        Model(pair polySize):
            polygonSize(polySize)
            {
                int nRabbits  = 0.02*polygonSize.first*polygonSize.second;
                int snakeSize = 0.01*polygonSize.first*polygonSize.second;
                Randomize(nRabbits, snakeSize);
            };

        ~Model()
            {};
        
        void Randomize(int nRabbits, int snakeSize);

        void AddRabbit(const Rabbit& rabbit)
            {
                rabbits.push_back(rabbit);
            }

        void AddSnake(const Snake& snake)
            {
                snakes.push_back(snake);
            }

        void Update(const pair& newPolygonSize);

        Rabbits& GetRabbits()
            {
                return rabbits;
            }

        Snakes& GetSnakes()
            {
                return snakes;
            }
};
