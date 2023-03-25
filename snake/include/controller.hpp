#pragma once


#include "view.hpp"
#include "model.hpp"


//----------------------------------------//

class Controller
{
    private:

        Snake* snake;

    public:

        Controller(Snake* snake_):
            snake(snake_) {}

        virtual ~Controller()
            {}

        Snake* GetSnake()
            {
                return snake;
            }

        void SetSnake(Snake* snake_)
            {
                snake = snake_;
            }

        virtual void OnKey(int key) = 0;
        virtual void OnTimer(int passedTime) = 0;
};

//----------------------------------------//

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

        void OnTimer(int passedTime) override
            {
                assert(passedTime);
            }
};

//----------------------------------------//

class BotController: public Controller
{
    public:

        BotController(Snake* snake_):
            Controller(snake_)
            {
                View* view = View::Get();
                view->SetOnTimer(std::bind(&BotController::OnTimer, this,
                                 std::placeholders::_1));
                Model* model = view->GetModel();
                victim = model->GetClosestRabbit(GetSnake()->GetFront());
            }

        void OnKey(int key) override
            {
                assert(key);
            }

        void OnTimer(int passedTime) override;

    private:

        Rabbit* victim;
};

//----------------------------------------//
