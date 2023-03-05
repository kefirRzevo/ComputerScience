#pragma once

#include <iostream>

#include "view.hpp"
#include "model.hpp"

class Controller
{
    private:

        Snake* snake;

    public:

        Controller() = default;

        Snake* GetSnake() const
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
    public:

        HumanController(Snake* snake_)
            {
                SetSnake(snake_);
                View* view = View::Get();
                view->SetOnKey(std::bind(&HumanController::OnKey, this, std::placeholders::_1));
            }

        void OnKey(int key) override;
};
