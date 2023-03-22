#pragma once


#include <iostream>
#include <vector>
#include <list>

class Rabbit;
class Snake;
class CleverSnake;
class Model;

using Size        = std::pair<size_t, size_t>;
using Coordinate  = std::pair<int, int>;
using Coordinates = std::list<Coordinate>;
using Rabbits = std::list<Rabbit* >;
using Snakes  = std::vector<Snake* >;


class Rabbit: public Coordinate
{
    private:

        size_t score;
        bool   killed;

    public:

        Rabbit(const Coordinate& coord_ = {}, int score_ = 1):
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

        void SetScore(int score_)
            {
                score = score_;
            }

        int GetScore() const
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

    public:

        Snake(const Coordinates& coords_ = {}, Direction dir_ = Direction::UP):
            coords(coords_), dir(dir_)
            {}

        bool RandomGenerate(Model* model, size_t snakeSize = 8);

        bool Update(Model* model);

        void Feed(Rabbit* rabbit)
            {
                scoreReserved += rabbit->GetScore();
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
};


class Model
{
    public:

        enum class GameMode
        {
            SINGULAR,
            MULTI,
        };

    private:

        GameMode mode;
        bool finished;
        int winner;

    public:

        Rabbits rabbits;
        Snakes snakes;

    private:

        Size polygonSize;

    public:

        Model(Size polygonSize_, GameMode mode_);

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

        int GetWinner() const
            {
                return winner;
            }

        void OnTimer(int passedTime);
};
