#include <sys/ioctl.h>
#include <sys/time.h> 
#include <termios.h>
#include <csignal>
#include <poll.h>
#include <cmath>

#include "../include/textview.hpp"

#ifdef DEBUG
#include <fstream>
std::ofstream fout("textview.txt");
#endif

void 
SigHandler(int signum);

static const int POLL_PERIOD = 10;

static const int CSI_fg_color_code = 30;
static const int CSI_bg_color_code = 40; 

static const int TOP_paddind = 3;
static const int BOTTOM_padding = 1;
static const int LEFT_paddind = 2;
static const int RIGHT_padding = 2;


//----------------------------------------//

static bool
IsInFrame(const Coordinate& frameSize, const Coordinate& point)
{
    if(point.first <= 0 || point.first + 2 > frameSize.first)
        return false;

    if(point.second <= 0 || point.second + 2 > frameSize.second)
        return false;

    return true;
}

//----------------------------------------//

TextView::TextView()
{
    finished = false;
    UpdateWindowSize();
    signal(SIGWINCH, SigHandler);
    signal(SIGINT, SigHandler);
    TermiosPropsOff();
    CarretOff();
    SetOnTimer({DRAW_TIME_MSEC, std::bind(&TextView::OnTimer, this)});
}

//----------------------------------------//

TextView::~TextView()
{
    signal(SIGWINCH, SIG_DFL);
    TermiosPropsOn();
    CarretOn();
}

//----------------------------------------//

void 
SigHandler(int signum)
{
    View* view = View::Get();
    if(!view)
        return;

    if(signum == SIGWINCH)
    {
        view->UpdateWindowSize();
    }

    if(signum == SIGINT)
    {
        view->finished = true;
    }
}

//----------------------------------------//

void
TextView::UpdateWindowSize()
{
    struct ttysize ts;
    ioctl(0, TIOCGSIZE, &ts);
    windowSize.first = ts.ts_cols;
    windowSize.second = ts.ts_lines;
    timeToDraw = true;
}

//----------------------------------------//

void
TextView::PollOnKey()
{
    struct pollfd requested = 
    {
        .fd = STDIN_FILENO, 
        .events = POLLIN, 
        .revents = 0
    };

    poll(&requested, 1, POLL_PERIOD);

    if (requested.revents & POLL_IN)
    {
        int symbol = 0;
        read(STDIN_FILENO, &symbol, 1);
        
        if(symbol == 'q')
        {
            finished = true;
        }

        for (const auto& action: listenersOnKey)
        {
            action(symbol);
        }
    }
}

//----------------------------------------//

void
TextView::OnTimer()
{
    if(model->SnakesAlive())
        timeToDraw = true;
}

void
TextView::RunLoop()
{
    timeval start  = {};
    timeval end    = {};
    timeval passed = {};
    int msecPassed = 0;

    while(1)
    {
        gettimeofday(&start, nullptr);
        PollOnKey();
        gettimeofday(&end, nullptr);

        timersub(&end, &start, &passed);
        msecPassed = passed.tv_sec * 1000 + passed.tv_usec / 1000;
        PollOnTimer(msecPassed);

        if(timeToDraw)
        {
            Clear();
            DrawFrame();
            DrawRabbits();
            DrawSnakes();
            fflush(stdout);
            timeToDraw = false;
        }

        if(finished)
            break;
    }
}

//----------------------------------------//

void
TextView::DrawRabbits() const
{
    for(const auto& rabbit: model->rabbits)
    {
        Coordinate rabbitCoord = rabbit->GetCoordinate();

        if(IsInFrame(frameSize, rabbitCoord))
        {
            rabbitCoord.first  += upLeftCorner.first;
            rabbitCoord.second += upLeftCorner.second;

            Symbol(rabbitCoord, 'R');
        }
    }
}

//----------------------------------------//

void
TextView::DrawSnakes() const
{
    for(const auto& snake: model->snakes)
    {
        if(!snake->IsInGame())
            continue;

        for(auto iterator  = ++snake->GetCoordinates().begin(); 
                 iterator != --snake->GetCoordinates().end(); ++iterator)
        {
            Coordinate section = *iterator;

            if(IsInFrame(frameSize, section))
            {            
                section.first  += upLeftCorner.first;
                section.second += upLeftCorner.second;

                Symbol(section, '0');
            }
        }

        Coordinate back = snake->GetBack();

        if(IsInFrame(frameSize, back))
        {
            back.first  += upLeftCorner.first;
            back.second += upLeftCorner.second;

            Symbol(back, 'B');
        }

        Coordinate front = snake->GetFront();
        if(IsInFrame(frameSize, front))
        {
            front.first  += upLeftCorner.first;
            front.second += upLeftCorner.second;

            Symbol(front, 'F');
        }
    }
}

//----------------------------------------//

void
TextView::DrawFrame()
{
    int viewWidth  = windowSize.first;
    int viewHeight = windowSize.second;

    int modelWidth  = model->GetPolygonSize().first;
    int modelHeight = model->GetPolygonSize().second;

    int frameWidth  = 0;
    int frameHeight = 0;

    int cornerUpLeftX = 0;
    int cornerUpLeftY = 0;

    if(viewWidth <= modelWidth + 2 + LEFT_paddind + RIGHT_padding)
    {
        frameWidth = viewWidth - LEFT_paddind - RIGHT_padding;
        cornerUpLeftX = LEFT_paddind + 1;
    }
    else
    {
        frameWidth = modelWidth + 2;
        cornerUpLeftX = (viewWidth - frameWidth) / 2 + 1;
    }

    if(viewHeight <= modelHeight + 2 + TOP_paddind + BOTTOM_padding)
    {
        frameHeight = viewHeight - TOP_paddind - BOTTOM_padding;
        cornerUpLeftY = TOP_paddind + 1;
    }
    else
    {
        frameHeight = modelHeight + 2;
        cornerUpLeftY = (viewHeight - frameHeight) / 2 + 1;
    }

    AddProperties(RED, WHITE);

    HLine(cornerUpLeftX, cornerUpLeftY, frameWidth, '=');
    HLine(cornerUpLeftX, cornerUpLeftY + frameHeight - 1, frameWidth, '=');
    VLine(cornerUpLeftX, cornerUpLeftY, frameHeight, '#');
    VLine(cornerUpLeftX + frameWidth - 1, cornerUpLeftY, frameHeight, '#');

    frameSize.first  = frameWidth;
    frameSize.second = frameHeight;

    upLeftCorner.first  = cornerUpLeftX;
    upLeftCorner.second = cornerUpLeftY;

    int resultsSize = results.size();
    UpdateResults();
    assert(resultsSize < viewWidth);
    String((viewWidth - resultsSize) / 2 + 1,
           (TOP_paddind - 1) / 2 + 1, results.c_str());
}

//----------------------------------------//

void
TextView::UpdateResults()
{
    results.clear();
    Model* model = GetModel();

    size_t i = 0;
    for(const auto& snake: model->snakes)
    {
        results += "Player";
        results += std::to_string(i++);
        results += " ";
        results += std::to_string(snake->GetScore());

        if(!snake->IsAlive())
            results += "(Died)";

        results += "; ";
    }
}

//----------------------------------------//

void
TextView::AddProperties(enum TextView::Color fg, enum TextView::Color bg) const
{
    printf("\e[%d;%dm", fg + CSI_fg_color_code, bg + CSI_bg_color_code);
}

//----------------------------------------//

void
TextView::HLine(int x0, int y0, int len, int sym) const
{
    for(int i = x0; i < x0 + len; i++)
        Symbol({i, y0}, sym);
}

//----------------------------------------//

void
TextView::VLine(int x0, int y0, int len, int sym) const
{
    for(int i = y0; i < y0 + len; i++)
        Symbol({x0, i}, sym);
}

//----------------------------------------//

void
TextView::Symbol(Coordinate coord, int sym) const
{
    printf("\e[%d;%dH", coord.second, coord.first);
    printf("%c", sym);
}

//----------------------------------------//

void
TextView::String(int x, int y, const char* string) const
{
    printf("\e[%d;%dH", y, x);
    printf("%s", string);
}

//----------------------------------------//

void
TextView::Clear() const
{
    printf("\e[2J");
}

//----------------------------------------//

void
TextView::CarretOff() const
{
    printf("\e[?25l");
}

//----------------------------------------//

void
TextView::CarretOn() const
{
    printf("\e[?25h");
}

//----------------------------------------//

void
TextView::TermiosPropsOn()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &(this->termis_attr));
}

//----------------------------------------//

void
TextView::TermiosPropsOff()
{
    tcgetattr(STDIN_FILENO, &(this->termis_attr));

    struct termios attr_raw = this->termis_attr;
    cfmakeraw(&attr_raw);
    attr_raw.c_lflag |= ISIG;

    tcsetattr(STDIN_FILENO, TCSANOW, &attr_raw);
}

//----------------------------------------//
