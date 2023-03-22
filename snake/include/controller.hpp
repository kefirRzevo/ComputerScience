#pragma once

#include <iostream>

#include "view.hpp"
#include "model.hpp"

class Controller
{
    private:

        Snake* snake;

    public:

        Controller(Snake* snake_):
            snake(snake_) {}

        Snake* GetSnake()
            {
                return snake;
            }

        void SetSnake(Snake* snake_)
            {
                snake = snake_;
            }

        virtual void OnKey(int key) = 0;
};

class HumanController: public Controller
{
    private:
    
        int leftKey;
        int rightKey;
        int upKey;
        int downKey;

    public:

        HumanController(Snake* snake_, int left, int right, int up, int down):
            Controller(snake_),
            leftKey(left), rightKey(right), upKey(up), downKey(down)
            {
                View* view = View::Get();
                view->SetOnKey(std::bind(&HumanController::OnKey, this,
                               std::placeholders::_1));
            }

        void OnKey(int key) override;
};
