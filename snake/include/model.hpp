#pragma once

#include <cassert>
#include <iostream>
#include <list>
#include <vector>

#include "config.hpp"

class Rabbit;
class Snake;
class Model;

using Size = std::pair<int, int>;
using Coordinate = std::pair<int, int>;
using Coordinates = std::list<Coordinate>;
using Rabbits = std::list<Rabbit *>;
using Snakes = std::list<Snake *>;

static FILE *fp = fopen("build/dump.txt", "w+");

#ifdef DEBUG
#define $$                                                                     \
  do {                                                                         \
    fprintf(fp, "%s %d\n", __FILE__, __LINE__);                                \
    fflush(fp);                                                                \
  } while (0);
#else
#define $$
#endif

//----------------------------------------//

class Rabbit : public Coordinate {
private:
  int score;
  bool alive;
  int styleType;

public:
  Rabbit(int styleType_) : score(1), alive(true), styleType(styleType_) {
    first = 0;
    second = 0;
  }

  void RandomGenerate(Model *model, int score_ = RABBIT_SCORE);

  bool operator==(const Rabbit &rhs) {
    return *static_cast<Coordinate *>(this) == rhs.GetCoordinate();
  }

  void SetCoordinate(const Coordinate &coord_) {
    this->first = coord_.first;
    this->second = coord_.second;
  }

  void SetStyleType(int styleType_) { styleType = styleType_; }

  Coordinate GetCoordinate() const { return static_cast<Coordinate>(*this); }

  int GetScore() const { return score; }

  int GetStyleType() const { return styleType; }

  void Kill() { alive = false; }

  void Hill(int score_ = 1) {
    score = score_;
    alive = true;
  }

  bool IsAlive() const { return alive; }
};

//----------------------------------------//

class Snake {
public:
  enum Direction {
    LEFT = 0,
    RIGHT = 1,
    UP = 2,
    DOWN = 3,
  };

private:
  Coordinates coords;
  Coordinate newFront;

  Direction dirInFuture;
  Direction dirAfterUpdate;

  int styleType;
  int scoreReserved;
  int score;

  bool alive;
  bool inGame;

public:
  Snake(int styleType_);

  void RandomGenerate(Model *model, int snakeSize = SNAKE_SIZE);

  void SetNewFront();

  void Update(Model *model);

  void Feed(int score_) {
    scoreReserved += score_;
    score += score_;
  }

  void Kill() { alive = false; }

  void SetCoordinates(const Coordinates &coords_) { coords = coords_; }

  const Coordinates &GetCoordinates() const { return coords; }

  void SetDirection(Direction dir) { dirInFuture = dir; }

  void SetStyleType(int styleType_) { styleType = styleType_; }

  int GetScore() const { return score; }

  Direction GetDirection() const { return dirAfterUpdate; }

  const Coordinate &GetNewFront() const { return newFront; }

  const Coordinate &GetFront() const { return coords.front(); }

  const Coordinate &GetBack() const { return coords.back(); }

  int GetStyleType() const { return styleType; }

  bool IsAlive() const { return alive; }

  bool IsInGame() const { return inGame; }

  void OnTimerDeath();

  void Dump() const;

  static Snake::Direction GetOppositeDir(Snake::Direction dir);

  static Coordinate GetCoordDirection(Snake::Direction dir);

private:
  void UpdateDirection() { dirAfterUpdate = dirInFuture; }
};

//----------------------------------------//

class Model {
private:
  Size polygonSize;
  bool snakesAlive;

public:
  Rabbits rabbits;
  Snakes snakes;

  Model(Size polygonSize_, size_t nSnakes = 2);

  ~Model();

  void Update();

  Snake *GetSnake(size_t index) {
    auto snakeIterator = snakes.begin();
    std::advance(snakeIterator, index);
    return *snakeIterator;
  }

  void DeleteSnake(Snake *snake);

  Size GetPolygonSize() const { return polygonSize; }

  bool SnakesAlive() const { return snakesAlive; }

  void SetPolygonSize(const Size &size_) { polygonSize = size_; }

  Coordinate GetClosestRabbitCoord(const Coordinate &point) const;

  void OnTimer();

  int GetSnakeIndex(const Snake *snake) const;

  bool IsThereRabbit(const Coordinate &point) const;

  static int GetDistance(const Coordinate &coord1, const Coordinate &coord2);
};

//----------------------------------------//
