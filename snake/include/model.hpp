#pragma once


#include <iostream>
#include <vector>
#include <list>

#define DEBUG

class Rabbit;
class Snake;
class Model;

using Size        = std::pair<int, int>;
using Coordinate  = std::pair<int, int>;
using Coordinates = std::list<Coordinate>;
using Rabbits = std::list<Rabbit* >;
using Snakes  = std::vector<Snake* >;


//----------------------------------------//

class Rabbit: public Coordinate
{
    private:

        size_t score;
        bool killed;

    public:

        Rabbit(const Coordinate& coord_ = {}, size_t score_ = 1):
            score(score_), killed(false)
            {
                static_cast<Coordinate>(*this) = coord_;
            }

        bool RandomGenerate(Model* model, size_t score_ = 1);

        bool operator==(const Rabbit& rhs)
            {
                return static_cast<Coordinate>(*this) == rhs.GetCoordinate();
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

        void SetScore(size_t score_)
            {
                score = score_;
            }

        size_t GetScore() const
            {
                return score;
            }

        void Kill()
            {
                killed = true;
            }

        void Hill()
            {
                killed = false;
            }

        bool IsKilled() const
            {
                return killed;
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
        size_t scoreReserved = 0;
        int score;
        bool alive;
        bool updated;

    public:

        Snake(const Coordinates& coords_ = {}, Direction dir_ = Direction::UP):
            coords(coords_), dir(dir_), score(0), alive(true), updated(false)
            {}

        bool RandomGenerate(Model* model, size_t snakeSize = 8);

        void Update(Model* model);

        void Feed(Rabbit* rabbit)
            {
                scoreReserved += rabbit->GetScore();
                score += static_cast<int>(rabbit->GetScore());
            }

        void Kill()
            {
                score = -1;
            }

        bool GetUpdated() const
            {
                return updated;
            }

        void UnsetUpdated()
            {
                updated = false;
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

        void SetDirection(Direction dir_)
            {
                dir = dir_;
            }

        Direction GetDirection() const
            {
                return dir;
            }

        Coordinate GetCoordinateDirection() const;

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
};

//----------------------------------------//

class Model
{
    public:

        enum class GameMode
        {
            SINGULAR,
            MULTI,
        };

    private:

        Size polygonSize;
        bool finished;

    public:

        Rabbits rabbits;
        Snakes snakes;

    public:

        Model(Size polygonSize_, size_t nSnakes = 2);

        ~Model();

        void Update();

        Snake* GetSnake(size_t index)
            {
                return (index < snakes.size() ? snakes[index] : nullptr);
            }

        Size GetPolygonSize() const
            {
                return polygonSize;
            }

        void SetPolygonSize(Size size_)
            {
                polygonSize = size_;
            }

        bool GameIsFinished() const
            {
                return finished;
            }

        Rabbit* GetClosestRabbit(const Coordinate& point);

        void OnTimer(int passedTime);
};

//----------------------------------------//
