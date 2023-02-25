#include <sys/ioctl.h>
#include <csignal>

#include "../include/textview.hpp"

#include<fstream>
std::ofstream fout("dump.txt");


static const int CSI_fg_color_code = 30;
static const int CSI_bg_color_code = 40; 

static const int TOP_paddind = 3;
static const int BOTTOM_padding = 1;
static const int LEFT_paddind = 2;
static const int RIGHT_padding = 2;

void SigHandler(int signum)
{
    View* view = View::Get();
    if(!view)
            return;

    if(signum == SIGWINCH)
    {
            view->UpdateWindowSize();
    }
    else if(signum == SIGINT)
    {
            //std::cout << "Goodbye!" << std::endl;
            view->UpdateStatus();
    }
}

TextView::TextView()
{
    UpdateWindowSize();
    status = true;

    signal(SIGWINCH, SigHandler);
    signal(SIGINT, SigHandler);
    CarretOff();
}

void TextView::UpdateWindowSize()
{
    struct ttysize ts;
    ioctl(0, TIOCGSIZE, &ts);
    windowSize.first = ts.ts_cols;
    windowSize.second = ts.ts_lines;
}

void TextView::AddProperties(enum TextView::Color fg, enum TextView::Color bg) const
{
        printf("\e[%d;%dm", fg + CSI_fg_color_code, bg + CSI_bg_color_code);
        fflush(stdout);
}

Coordinate TextView::ModelCoordToView(const Coordinate& modelCoord) const
{
    const size& polygonSize = GetModel()->GetPolygonSize();
    const size& windowSize = GetWindowSize();
    Coordinate viewCoord = {-1, 0};

    if(modelCoord.first > static_cast<int>(polygonSize.first) || modelCoord.first < 0)
        return viewCoord;

    if(modelCoord.second > static_cast<int>(polygonSize.second) || modelCoord.second < 0)
        return viewCoord;

    double relativeX = (double)modelCoord.first/polygonSize.first;
    double relativeY = (double)modelCoord.second/polygonSize.second;
    
    viewCoord.first = relativeX * (windowSize.first - LEFT_paddind - RIGHT_padding - 2) + LEFT_paddind + 1;
    viewCoord.second = relativeY * (windowSize.second - TOP_paddind - BOTTOM_padding - 2) + TOP_paddind + 1;
    return viewCoord;
}

void TextView::DrawRabbits() const
{
    Rabbits& rabbits = GetModel()->GetRabbits();
    for(const auto& rabbit: rabbits)
    {
        Coordinate rabbitViewCoord = ModelCoordToView(rabbit.SeeCoordinate());
        if(rabbitViewCoord.first == -1)
            continue;

        Symbol(rabbitViewCoord, '@');
    }
}


void TextView::DrawSnakes() const
{
    Snakes& snakes = GetModel()->GetSnakes();
    for(const auto& snake: snakes)
    {
        const Coordinates& snakeCoords = snake.SeeCoordinates();
        const Coordinate& viewTail = ModelCoordToView(snakeCoords.back());
        const Coordinate& viewHead = ModelCoordToView(snakeCoords.front());

        for(auto i = snakeCoords.begin(); i != snakeCoords.end(); i++)
        {
            const auto& j = std::next(i);

            if(j == snakeCoords.end())
                continue;

            Coordinate jView = ModelCoordToView(*j);
            Coordinate iView = ModelCoordToView(*i);

            if(iView.first == -1)
                continue;

            if((*i).first == (*j).first && (abs(iView.second - jView.second) != 1))
            {
                int min = std::min(iView.second, jView.second);
                int max = std::max(iView.second, jView.second);

                for(int k = min + 1; k < max; k++)
                    Symbol({iView.first, k}, '?');
            }
            else if((*i).second == (*j).second && (abs(iView.first - jView.first) != 1))
            {
                int min = std::min(iView.first, jView.first);
                int max = std::max(iView.first, jView.first); 

                for(int k = min + 1; k < max; k++)
                    Symbol({k, iView.second}, '?');
            }

            Symbol(iView, '?');
        }

        if(viewTail.first != -1)
            Symbol(viewTail, 'T');

        if(viewHead.first != -1)
            Symbol(viewHead, 'H');
    }
}

void TextView::DrawFrame() const
{
    assert(GetModel());

    int width = GetWindowSize().first;
    int height = GetWindowSize().second;

    AddProperties(RED, WHITE);
    HLine(LEFT_paddind + 1, TOP_paddind + 1, width - LEFT_paddind - RIGHT_padding, '=');
    HLine(LEFT_paddind + 1, height - BOTTOM_padding, width - LEFT_paddind - RIGHT_padding, '=');
    VLine(LEFT_paddind + 1, TOP_paddind + 1, height - TOP_paddind - BOTTOM_padding, '#');
    VLine(width - RIGHT_padding, TOP_paddind + 1, height - TOP_paddind - BOTTOM_padding, '#');
    std::string name = "Hello world!";
    String((width - name.size()) / 2, (TOP_paddind - 1) / 2 + 1, name);

    fflush(stdout);
}
