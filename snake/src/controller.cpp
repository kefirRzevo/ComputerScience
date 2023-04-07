#include "../include/controller.hpp"

#include <algorithm>
#include <vector>

#define DEBUG


static Coordinate
GetShift(const Coordinate& coord, Snake::Direction dir);

bool
IsPointSecure(const Coordinate& point);


class Route
{
    public:

        Snake::Direction dir;
        Coordinate newPos;
        int dist;
        bool secure;

        Route() = default;

        void Create(Snake::Direction dir_, const Coordinate& curPos, const Coordinate& target)
            {
                dir = dir_;
                newPos = GetShift(curPos, dir);
                secure = IsPointSecure(newPos);
                dist = Model::GetDistance(newPos, target);
            }
};

struct RouteCompare
{
    bool operator()( Route* lx, Route* rx ) const
    {
        return lx->dist < rx->dist;
    }
};

class RouteTree
{
    private:

        Snake::Direction curDir;
        Coordinate target;
        Route routes[3];

    public:

        RouteTree(const Coordinate& target_, const Coordinate& pos_, Snake::Direction dir_):
            curDir(dir_), target(target_)
            {
                int i = 0;
                for(int j = 0; j < 4; j++)
                {
                    Snake::Direction dir = static_cast<Snake::Direction>(j);
                    if(dir == Snake::GetOppositeDir(dir_))
                        continue;

                    routes[i++].Create(dir, pos_, target_);
                    fprintf(fp, "|dir %d|dist %d| secure %d|\n", j, routes[i-1].dist, routes[i-1].secure);
                }
            }

        Snake::Direction GetDirection()
            {
                std::vector<Route*> secureRoutes;
                for(int i = 0; i < 3; i++)
                    if(routes[i].secure)
                        secureRoutes.push_back(&routes[i]);

                if(secureRoutes.size() > 0)
                {
                    std::sort(secureRoutes.begin(), secureRoutes.end(), RouteCompare());
                    for(int i = secureRoutes.size() - 1; i >= 0; i--)
                        if(secureRoutes[i]->dist > secureRoutes[0]->dist)
                            secureRoutes.pop_back();

                    return secureRoutes[std::rand() % secureRoutes.size()]->dir;
                }

                return curDir;
            }
};

//----------------------------------------//

static Coordinate
GetShift(const Coordinate& coord, Snake::Direction dir)
{
    Coordinate dirCoord = Snake::GetCoordDirection(dir);
    return {coord.first + dirCoord.first, coord.second + dirCoord.second};
}

//----------------------------------------//

void 
HumanController::OnKey(KeyCode key)
{
    if(!snake->IsAlive())
        return;

    Snake::Direction dir = snake->GetDirection();
    Snake::Direction newDir = dir;

    if(key == leftKey)
        newDir = Snake::LEFT;
    else if(key == rightKey)
        newDir = Snake::RIGHT;
    else if(key == upKey)
        newDir = Snake::UP;
    else if(key == downKey)
        newDir = Snake::DOWN;

    if(newDir == dir || newDir == Snake::GetOppositeDir(dir))
        return;

    snake->SetDirection(newDir);
}

//----------------------------------------//

bool
IsPointSecure(const Coordinate& point)
{
    Model* model = View::Get()->GetModel();
    for(const auto& snake: model->snakes)
    {
        if(!snake->IsInGame())
            continue;

        for(const auto& section: snake->GetCoordinates())
            if(section == point)
                return false;
    }

    return true;
}

//----------------------------------------//

void
BotController::OnTimer()
{
    if(!snake->IsAlive())
        return;

    Model* model = View::Get()->GetModel();
    Coordinate front = snake->GetFront();

    if(!model->IsThereRabbit(target))
        target = model->GetClosestRabbitCoord(front);
fprintf(fp, "\n\n");
    fprintf(fp, "|x %d|y %d| target %d|%d|\n", snake->GetFront().first, snake->GetFront().second, target.first, target.second);
    RouteTree tree{target, snake->GetFront(), snake->GetDirection()};
    Snake::Direction a = tree.GetDirection();
    fprintf(fp, "newDir %d\n", a);
    fflush(fp);
    snake->SetDirection(a);
}

//----------------------------------------//
