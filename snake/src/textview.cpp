#include <sys/ioctl.h>
#include <termios.h>
#include <csignal>
#include <poll.h>
#include <cmath>

#include "../include/textview.hpp"


#include <fstream>
std::ofstream fout("dump.txt");

//----------------------------------------

static struct termios* 
TermiosAttrs();

void 
SigHandler(int signum);

static const int POLL_PERIOD = 10;

static const int CSI_fg_color_code = 30;
static const int CSI_bg_color_code = 40; 

static const int TOP_paddind = 3;
static const int BOTTOM_padding = 1;
static const int LEFT_paddind = 2;
static const int RIGHT_padding = 2;



static Coordinate
GetSnakeFocus(Snake* snake)
{
    assert(snake);

    const Coordinates& coords = snake->GetCoordinates();
    assert(coords.size() != 0);

    int sumX = 0;
    int sumY = 0;

    for(const auto& section: coords)
    {
        sumX += section.first;
        sumY += section.second;
    }

    return {sumX/coords.size(), sumY/coords.size()};
}

static Coordinate
GetFocus(Model* model)
{
    Coordinate focus = {};

    if(model->snakes.size() == 0)
    {
        focus.first  = model->GetPolygonSize().first  / 2;
        focus.second = model->GetPolygonSize().second / 2;
        return focus;
    }

    for(const auto& snake: model->snakes)
    {
        focus.first  += GetSnakeFocus(snake).first;
        focus.second += GetSnakeFocus(snake).second;
    }

    focus.first  /= model->snakes.size();
    focus.second /= model->snakes.size();

    return focus;
}

static bool
IsInFrame(const Coordinate& frameSize, const Coordinate& point)
{
    if(point.first <= 0 || point.first + 2 > frameSize.first)
        return false;
    
    if(point.second <= 0 || point.second + 2 > frameSize.second)
        return false;

    return true;
}



//--------------------GENERAL--------------------

TextView::TextView()
{
    finished = false;
    UpdateWindowSize();
    signal(SIGWINCH, SigHandler);
    signal(SIGINT, SigHandler);
    TermiosPropsOff();
    CarretOff();
}

TextView::~TextView()
{
    signal(SIGWINCH, SIG_DFL);
    TermiosPropsOn();
    CarretOn();
}

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

void
TextView::UpdateWindowSize()
{
    struct ttysize ts;
    ioctl(0, TIOCGSIZE, &ts);
    windowSize.first = ts.ts_cols;
    windowSize.second = ts.ts_lines;
}

void
TextView::PollOnKey()
{
    struct pollfd requested = {.fd = STDIN_FILENO, .events = POLLIN, .revents = 0};

    poll(&requested, 1, POLL_PERIOD);

    if (requested.revents & POLL_IN)
    {
        unsigned char symbol = 0;
        read(STDIN_FILENO, &symbol, 1);
        
        if(symbol == 'q')
        {
            finished = true;
        }

        for (const auto& action: listenersOnKey)
        {
            action((int) symbol);
        }
    }
}

void
TextView::RunLoop()
{
    while(1)
    {
        DrawFrame();
        DrawRabbits();
        DrawSnakes();
        fflush(stdout);
        usleep(400000);
        if(finished)
            break;
        Clear();
        PollOnKey();
        model->Update();
    }
}


//--------------------DRAWING--------------------

void
TextView::RedrawRabbits() const
{

}

void
TextView::RedrawSnakes() const
{

}

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

void
TextView::DrawSnakes() const
{
    for(const auto& snake: model->snakes)
    {
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

void
TextView::DrawFrame()
{
    size_t viewWidth  = windowSize.first;
    size_t viewHeight = windowSize.second;

    size_t modelWidth  = model->GetPolygonSize().first;
    size_t modelHeight = model->GetPolygonSize().second;

    size_t frameWidth  = 0;
    size_t frameHeight = 0;

    int cornerUpLeftX = 0;
    int cornerUpLeftY = 0;

    frameFull = true;
    if(viewWidth <= modelWidth + 2 + LEFT_paddind + RIGHT_padding)
    {
        frameWidth = viewWidth - LEFT_paddind - RIGHT_padding;
        cornerUpLeftX = LEFT_paddind + 1;
        frameFull = false;
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
        frameFull = false;
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

    {
        fout << "corner: " << upLeftCorner.first << " " << upLeftCorner.second << "\n";
        fout << "sizes:  " << frameWidth << " "<<frameHeight << "\n";
    }
    std::string name = "Hello world!";
    String((viewWidth - name.size()) / 2 + 1, (TOP_paddind - 1) / 2 + 1, name);

    fflush(stdout);
}


//--------------------BASIC DRAWING--------------------

void
TextView::AddProperties(enum TextView::Color fg, enum TextView::Color bg) const
{
    printf("\e[%d;%dm", fg + CSI_fg_color_code, bg + CSI_bg_color_code);
    fflush(stdout);
}

void
TextView::HLine(int x0, int y0, int len, int sym) const
{
    for(int i = x0; i < x0 + len; i++)
        Symbol({i, y0}, sym);
}

void
TextView::VLine(int x0, int y0, int len, int sym) const
{
    for(int i = y0; i < y0 + len; i++)
        Symbol({x0, i}, sym);
}

void
TextView::Symbol(Coordinate coord, int sym) const
{
    printf("\e[%d;%dH", coord.second, coord.first);
    printf("%c", sym);
}

void
TextView::String(int x, int y, const std::string& string) const
{
    printf("\e[%d;%dH", y, x);
    printf("%s", string.c_str());
}

void
TextView::Clear() const
{
    printf("\e[2J");
}


//--------------------SET AND UNSET DEFAULT PARAMETERS--------------------

void
TextView::CarretOff() const
{
    printf("\e[?25l");
}

void
TextView::CarretOn() const
{
    printf("\e[?25h");
}

struct termios* 
TermiosAttrs()
{
    static struct termios termios_attr;
    return &termios_attr;
}

void
TextView::TermiosPropsOn()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &(this->termis_attr));
}

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