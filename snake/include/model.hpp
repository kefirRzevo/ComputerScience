#pragma once


#include <iostream>
#include <vector>
#include <list>

#include "config.hpp"

#define DEBUG

class Rabbit;
class Snake;
class Model;

using Size        = std::pair<int, int>;
using Coordinate  = std::pair<int, int>;
using Coordinates = std::list<Coordinate>;
using Rabbits = std::list<Rabbit* >;
using Snakes  = std::list<Snake* >;


//----------------------------------------//

class Rabbit: public Coordinate
{
    private:

        int score;
        bool alive;

    public:

        Rabbit(const Coordinate& coord_ = {}, int score_ = 1):
            score(score_), alive(true)
            {
                *static_cast<Coordinate* >(this) = coord_;
            }

        void RandomGenerate(Model* model, int score_ = RABBIT_SCORE);

        bool operator==(const Rabbit& rhs)
            {
                return *static_cast<Coordinate* >(this) == rhs.GetCoordinate();
            }

        void SetCoordinate(const Coordinate& coord_)
            {
                this->first  = coord_.first;
                this->second = coord_.second;
            }

        Coordinate GetCoordinate() const
            {
                return static_cast<Coordinate>(*this);
            }

        int GetScore() const
            {
                return score;
            }

        void Kill()
            {
                alive = false;
            }

        void Hill(int score_ = 1)
            {
                score = score_;
                alive = true;
            }

        bool IsAlive() const
            {
                return alive;
            }
};

//----------------------------------------//

class Snake
{
    public:

        enum Direction
        {
            LEFT  = 0,
            RIGHT = 1,
            UP    = 2,
            DOWN  = 3,
        };

    private:

        Coordinates coords;
        Direction dir;

        Direction dirAfterUpdate;
        Coordinate newFront;

        int scoreReserved;
        int score;

        bool alive;
        bool inGame;

    public:

        Snake(const Coordinates& coords_ = {}, Direction dir_ = Direction::UP);

        void RandomGenerate(Model* model, int snakeSize = SNAKE_SIZE);

        void SetNewFront();

        void Update(Model* model);

        void Feed(int score_)
            {
                scoreReserved += score_;
                score += score_;
            }

        void Kill()
            {
                alive = false;
            }

        int GetScore() const
            {
                return score;
            }

        void SetCoordinates(const Coordinates& coords_)
            {
                coords = coords_;
            }

        const Coordinates& GetCoordinates() const
            {
                return coords;
            }

        void SetDirection(Direction dir_);

        Direction GetDirection() const
            {
                return dir;
            }

        Direction GetDirAfterUpdate() const
            {
                return dirAfterUpdate;
            }

        static Coordinate GetCoordDirection(Snake::Direction dir);

        const Coordinate& GetNewFront() const
            {
                return newFront;
            }

        const Coordinate& GetFront() const
            {
                return coords.front();
            }

        const Coordinate& GetBack() const
            {
                return coords.back();
            }

        bool IsAlive() const
            {
                return alive;
            }

        bool IsInGame() const
            {
                return inGame;
            }

        void OnTimerDeath();

        void Dump() const;
};

//----------------------------------------//

class Model
{
    private:

        Size polygonSize;
        bool snakesAlive;

    public:

        Rabbits rabbits;
        Snakes snakes;

        Model(Size polygonSize_, size_t nSnakes = 2);

        ~Model();

        void Update();

        Snake* GetSnake(size_t index)
            {
                auto snakeIterator = snakes.begin();
                std::advance(snakeIterator, index);
                return *snakeIterator;
            }

        void DeleteSnake(Snake* snake);

        Size GetPolygonSize() const
            {
                return polygonSize;
            }

        bool SnakesAlive() const
            {
                return snakesAlive;
            }

        void SetPolygonSize(const Size& size_)
            {
                polygonSize = size_;
            }

        Coordinate GetClosestRabbitCoord(const Coordinate& point) const;

        void OnTimer();

        int GetSnakeIndex(const Snake* snake) const;

        bool IsThereRabbit(const Coordinate& point) const;

        static int GetDistance(const Coordinate& coord1,
                               const Coordinate& coord2);
};

//----------------------------------------//
