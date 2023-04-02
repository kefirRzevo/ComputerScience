#include <sys/ioctl.h>
#include <sys/time.h> 
#include <termios.h>
#include <csignal>
#include <poll.h>
#include <cmath>

#include "../include/textview.hpp"
#include "../include/config.hpp"

#ifdef DEBUG
#include <fstream>
std::ofstream fout("textview.txt");
#endif

void 
SigHandler(int signum);

static const int CSI_fg_color_code = 30;
static const int CSI_bg_color_code = 40; 


//----------------------------------------//

TextView::TextView()
{
    finished = false;
    UpdateWindowSize();
    signal(SIGWINCH, SigHandler);
    signal(SIGINT, SigHandler);
    TermiosPropsOff();
    CarretOff();
    SetOnTimer({TICK_MSEC * 1000, std::bind(&TextView::OnTimer, this)});
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

bool
TextView::IsInFrame(const Coordinate& point) const
{
    if(point.first <= 0 || point.first + 2 > frameSize.first)
        return false;

    if(point.second <= 0 || point.second + 2 > frameSize.second)
        return false;

    return true;
}

//----------------------------------------//

int
TextView::GetPolledKey() const
{
    struct pollfd requested = 
    {
        .fd = STDIN_FILENO, 
        .events = POLLIN, 
        .revents = 0
    };

    poll(&requested, 1, TICK_MSEC / 10);

    if (requested.revents & POLL_IN)
    {
        int symbol = 0;
        read(STDIN_FILENO, &symbol, 1);
        return symbol;
    }

    return 0;
}

//----------------------------------------//

void
TextView::OnTimer()
{
    if(model->SnakesAlive())
        timeToDraw = true;
    else
        timeToDraw = false;
}

//----------------------------------------//

void
TextView::RunLoop()
{
    timeval start  = {};
    timeval end    = {};
    timeval passed = {};

    gettimeofday(&start, nullptr);
    while(1)
    {
        PollOnKey(GetPolledKey());

        gettimeofday(&end, nullptr);
        timersub(&end, &start, &passed);
        PollOnTimer(passed.tv_sec * 1000000 + passed.tv_usec);
        gettimeofday(&start, nullptr);

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

        if(IsInFrame(rabbitCoord))
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

            if(IsInFrame(section))
            {            
                section.first  += upLeftCorner.first;
                section.second += upLeftCorner.second;

                Symbol(section, '0');
            }
        }

        Coordinate back = snake->GetBack();

        if(IsInFrame(back))
        {
            back.first  += upLeftCorner.first;
            back.second += upLeftCorner.second;

            Symbol(back, 'B');
        }

        Coordinate front = snake->GetFront();
        if(IsInFrame(front))
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

    if(viewWidth <= modelWidth + 2 + PADDING_LEFT + PADDING_RIGHT)
    {
        frameWidth = viewWidth - PADDING_LEFT - PADDING_RIGHT;
        cornerUpLeftX = PADDING_LEFT + 1;
    }
    else
    {
        frameWidth = modelWidth + 2;
        cornerUpLeftX = (viewWidth - frameWidth) / 2 + 1;
    }

    if(viewHeight <= modelHeight + 2 + PADDING_TOP + PADDING_BOTTOM)
    {
        frameHeight = viewHeight - PADDING_TOP - PADDING_BOTTOM;
        cornerUpLeftY = PADDING_TOP + 1;
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
           (PADDING_TOP - 1) / 2 + 1, results.c_str());
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
