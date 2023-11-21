#pragma once

#include <cassert>

#include "model.hpp"
#include "view.hpp"

//----------------------------------------//

class Controller {
protected:
  Snake *snake;

public:
  Controller(Snake *snake_) : snake(snake_) {}

  virtual ~Controller() {}

  Snake *GetSnake() { return snake; }

  void SetSnake(Snake *snake_) { snake = snake_; }

  virtual void OnKey(KeyCode key) = 0;
  virtual void OnTimer() = 0;
};

//----------------------------------------//

class HumanController : public Controller {
private:
  KeyCode leftKey;
  KeyCode rightKey;
  KeyCode upKey;
  KeyCode downKey;

public:
  HumanController(Snake *snake_, KeyCode left, KeyCode right, KeyCode up,
                  KeyCode down)
      : Controller(snake_), leftKey(left), rightKey(right), upKey(up),
        downKey(down) {
    View *view = View::Get();
    view->SetOnKey(
        std::bind(&HumanController::OnKey, this, std::placeholders::_1));
  }

  void OnKey(KeyCode key) override;

  void OnTimer() override {}
};

//----------------------------------------//

class BotController : public Controller {
public:
  BotController(Snake *snake_) : Controller(snake_), target({}) {
    View *view = View::Get();
    Model *model = view->GetModel();
    view->SetOnTimer(
        {BOT_TICK_MSEC * 1000, std::bind(&BotController::OnTimer, this)});
    target = model->GetClosestRabbitCoord(snake_->GetFront());
  }

  void OnKey(KeyCode key) override { assert(key); }

  void OnTimer() override;

private:
  Coordinate target;
};

//----------------------------------------//
