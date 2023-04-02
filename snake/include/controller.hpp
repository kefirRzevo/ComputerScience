#pragma once


#include "view.hpp"
#include "model.hpp"


using DirPriority = std::pair<int, Snake::Direction>;

//----------------------------------------//

class Controller
{
    protected:

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
        virtual void OnTimer() = 0;
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

        void OnTimer() override
            {}
};

//----------------------------------------//

class BotController: public Controller
{
    public:

        BotController(Snake* snake_):
            Controller(snake_), target({})
            {
                View* view = View::Get();
                Model* model = view->GetModel();
                view->SetOnTimer({TICK_MSEC * 1000, 
                                 std::bind(&BotController::OnTimer, this)});
                target = model->GetClosestRabbitCoord(snake_->GetFront());

                dirPriors[0].second = Snake::LEFT;
                dirPriors[1].second = Snake::RIGHT;
                dirPriors[2].second = Snake::UP;
                dirPriors[3].second = Snake::DOWN;
            }

        void OnKey(int key) override
            {
                assert(key);
            }

        void OnTimer() override;

    private:

        Coordinate target;
        DirPriority dirPriors[4];

        bool IsPointSecure(const Coordinate& point) const;

        void GetDirsPriority();
};

//----------------------------------------//
