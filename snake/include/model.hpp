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
        Rabbit() = default;

        Rabbit(const Coordinate& coord):
            coordinate(coord) {}

        Rabbit& operator=(const Coordinate& coord)
            {
                coordinate = coord;
                return *this;
            }

        bool operator==(const Rabbit& rhs)
            {
                return coordinate == rhs.coordinate;
            }

        Coordinate& SetCoordinate()
            {
                return coordinate;
            }

        const Coordinate& GetCoordinate() const
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
        enum Direction
            {
                UP    = 0,
                RIGHT = 1,
                DOWN  = 2,
                LEFT  = 3,
            };

        Snake() = default;

        Snake(const Coordinates& coords, Direction dir):
            coordinates(coords)
            {
                SetDirection(dir);
            }

        void Generate(size_t snakeSize, const size& polygonSize);

        void Update(const Coordinate& polygonSize);

        Coordinates& SetCoordinates()
            {
                return coordinates;
            }

        const Coordinates& GetCoordinates() const
            {
                return coordinates;
            }

        void SetDirection(Direction dir);

        void TurnLeft();

        void TurnRight();

        const Coordinate& GetDirection() const
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
        Snakes snakes;
        size polygonSize;

    public:
        Model() = delete;

        Model(size polySize, int nSnakes = 2);

        void Randomize(size_t nRabbits, sizes snakeSizes);

        void Update();

        const Rabbits& GetRabbits() const
            {
                return rabbits;
            }

        Rabbits& SetRabbits()
            {
                return rabbits;
            }

        const Snakes& GetSnakes() const
            {
                return snakes;
            }

        Snakes& SetSnakes()
            {
                return snakes;
            }

        Snake* GetSnake(size_t index)
            {
                return (index < snakes.size() ? &snakes[index] : nullptr);
            }

        size GetPolygonSize() const
            {
                return polygonSize;
            }

        void SetPolygonSize(size size_)
            {
                polygonSize = size_;
            }
};
