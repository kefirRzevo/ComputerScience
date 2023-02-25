#pragma once

#include <iostream>
#include <vector>
#include <list>

using size        = std::pair<size_t, size_t>;
using sizes       = std::vector<size_t>;
using Coordinate  = std::pair<int, int>;
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

        Coordinate& GetCoordinate()
            {
                return coordinate;
            }

        const Coordinate& SeeCoordinate() const
            {
                return coordinate;
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

        Snake& operator=(const Snake& snake)
            {
                coordinates = snake.coordinates;
                direction = snake.direction;
                return *this;
            }

        Snake() = default;

        ~Snake()
            {};

        Coordinates& GetCoordinates()
            {
                return coordinates;
            }

        Coordinate& GetDirection()
            {
                return direction;
            }

        const Coordinates& SeeCoordinates() const
            {
                return coordinates;
            }

        const Coordinate& SeeDirection() const
            {
                return direction;
            }
};

using Rabbits = std::vector<Rabbit>;
using Snakes  = std::vector<Snake>;

class Model
{
    private:
        Rabbits rabbits;
        Snakes  snakes;
        size polygonSize;

    public:
        Model() = delete;

        Model(size polySize, int nSnakes = 3):
            polygonSize(polySize)
            {
                int nRabbits    = 10;
                sizes snakeSizes(nSnakes, 8);
                Randomize(nRabbits, snakeSizes);
            };

        ~Model()
            {};
        
        void Randomize(size_t nRabbits, sizes snakeSizes);

        void Update();

        Rabbits& GetRabbits()
            {
                return rabbits;
            }

        Snakes& GetSnakes()
            {
                return snakes;
            }

        size GetPolygonSize() const
            {
                return polygonSize;
            }
};
