#include "../include/controller.hpp"

#include <algorithm>
#include <vector>

#define DEBUG
#define $ fprintf(stderr, "%d\n", __LINE__);

class Route;
struct RouteCompare;

static Coordinate GetShift(const Coordinate &coord, Snake::Direction dir);

bool IsPointSecure(const Coordinate &point);

class Route {
public:
  Snake::Direction dir;
  Coordinate pos;
  int dist;
  bool secure;
  Route *child[3];
  Route *bestChild;

  Route() = default;
  Route(const Route &) = delete;
  Route &operator=(const Route &) = delete;

  ~Route() {
    for (size_t i = 0; i < 3; i++)
      delete child[i];
  }

  void Create(Snake::Direction dir_, const Coordinate &parentPos,
              const Coordinate &target, int deep) {
    dir = dir_;
    pos = parentPos;
    dist = Model::GetDistance(pos, target);
    secure = IsPointSecure(pos);

    if (deep == 0) {
      for (size_t i = 0; i < 3; i++)
        child[i] = nullptr;

      bestChild = this;
      return;
    }

    int j = 0;
    for (int i = 0; i < 4; i++) {
      Snake::Direction childDir = static_cast<Snake::Direction>(i);
      if (childDir == Snake::GetOppositeDir(dir))
        continue;

      child[j] = new Route();
      child[j++]->Create(childDir, GetShift(parentPos, childDir), target,
                         deep - 1);
    }
    bestChild = nullptr;
  }

  void SetBestChild();
};

struct RouteCompare {
  bool operator()(Route *lx, Route *rx) const { return lx->dist < rx->dist; }
};

void Route::SetBestChild() {
  if (!child[0])
    return;

  std::vector<Route *> secureChildren;
  for (int i = 0; i < 3; i++) {
    child[i]->SetBestChild();
    if (child[i]->bestChild)
      if (child[i]->bestChild->secure && child[i]->secure)
        secureChildren.push_back(child[i]);
  }

  if (secureChildren.size() > 0) {
    std::sort(secureChildren.begin(), secureChildren.end(), RouteCompare());

    for (int i = secureChildren.size() - 1; i >= 0; i--)
      if (secureChildren[i]->dist > secureChildren[0]->dist)
        secureChildren.pop_back();
    bestChild = secureChildren[std::rand() % secureChildren.size()];
    return;
  }
  bestChild = child[std::rand() % 3];
}

class RouteTree {
private:
  Coordinate target;
  Route route;

public:
  RouteTree(Snake::Direction snakeDir_, const Coordinate &target_,
            const Coordinate &snakePos_, int deep_ = 5)
      : route() {
    route.Create(snakeDir_, snakePos_, target_, deep_);
  }

  Snake::Direction GetDirection() {
    route.SetBestChild();
    return route.bestChild->dir;
  }
};

//----------------------------------------//

static Coordinate GetShift(const Coordinate &coord, Snake::Direction dir) {
  Coordinate dirCoord = Snake::GetCoordDirection(dir);
  return {coord.first + dirCoord.first, coord.second + dirCoord.second};
}

//----------------------------------------//

void HumanController::OnKey(KeyCode key) {
  if (!snake->IsAlive())
    return;

  Snake::Direction dir = snake->GetDirection();
  Snake::Direction newDir = dir;

  if (key == leftKey)
    newDir = Snake::LEFT;
  else if (key == rightKey)
    newDir = Snake::RIGHT;
  else if (key == upKey)
    newDir = Snake::UP;
  else if (key == downKey)
    newDir = Snake::DOWN;

  if (newDir == dir || newDir == Snake::GetOppositeDir(dir))
    return;

  snake->SetDirection(newDir);
}

//----------------------------------------//

bool IsPointSecure(const Coordinate &point) {
  Model *model = View::Get()->GetModel();
  for (const auto &snake : model->snakes) {
    if (!snake->IsInGame())
      continue;
    snake->Dump();
    for (const auto &section : snake->GetCoordinates())
      if (section == point)
        return false;
  }

  return true;
}

//----------------------------------------//

void BotController::OnTimer() {
  if (!snake->IsAlive())
    return;

  Model *model = View::Get()->GetModel();
  Coordinate front = snake->GetFront();

  if (!model->IsThereRabbit(target))
    target = model->GetClosestRabbitCoord(front);

  // fprintf(fp, "\n\ndir %d|x %d|y %d| target %d|%d|\n",snake->GetDirection(),
  // snake->GetFront().first, snake->GetFront().second, target.first,
  // target.second);
  RouteTree tree{snake->GetDirection(), target, snake->GetFront()};
  Snake::Direction a = tree.GetDirection();
  // fprintf(fp, "newDir %d\n", a);
  fflush(fp);
  snake->SetDirection(a);
}

//----------------------------------------//
