#include <sys/ioctl.h>
#include <termios.h>
#include <csignal>
#include <poll.h>

#include "../include/textview.hpp"


//#include <fstream>
//std::ofstream fout("dump.txt");

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


//--------------------GENERAL--------------------

TextView::TextView()
{
    UpdateWindowSize();
    status = true;

    signal(SIGWINCH, SigHandler);
    signal(SIGINT, SigHandler);
    TermiosPropsOff();
    CarretOff();
}

TextView::~TextView()
{
    signal(SIGWINCH, SIG_DFL);
    signal(SIGINT, SIG_DFL);
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
    else if(signum == SIGINT)
    {
            view->UpdateStatus();
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
TextView::UpdateStatus()
{
    status = !status;
}

void 
TextView::PollOnKey() const
{
    struct pollfd requested = {.fd = STDIN_FILENO, .events = POLLIN, .revents = 0};

    poll(&requested, 1, POLL_PERIOD);

    if (requested.revents & POLL_IN)
    {
        char symbol = 0;
        read(STDIN_FILENO, &symbol, 1);

        for (const auto& action: listenersOnKey)
        {
            action((int) symbol);
        }
    }
}

void
TextView::Run()
{
    std::cout << "by\n";
    while(1)
    {
        Clear();
        DrawRabbits();
        DrawSnakes();
        DrawFrame();
        usleep(400000);
        if(!status)
            break;
        PollOnKey();
        GetModel()->Update();
    }
}

Coordinate
TextView::ModelCoordToView(const Coordinate& modelCoord) const
{
    const size& polygonSize = model->GetPolygonSize();
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

    if(viewCoord.first > static_cast<int>(windowSize.first) || viewCoord.first < 0)
        return {-1, 0};
    
    if(viewCoord.second > static_cast<int>(windowSize.second) || viewCoord.second < 0)
        return {-1, 0};

    return viewCoord;
}


//--------------------DRAWING--------------------

void
TextView::DrawRabbits() const
{
    const Rabbits& rabbits = model->GetRabbits();
    for(const auto& rabbit: rabbits)
    {
        Coordinate rabbitViewCoord = ModelCoordToView(rabbit.GetCoordinate());
        if(rabbitViewCoord.first == -1)
            continue;

        Symbol(rabbitViewCoord, '@');
    }
}

void
TextView::DrawSnakes() const
{
    const Snakes& snakes = model->GetSnakes();
    for(const auto& snake: snakes)
    {
        const Coordinates& snakeCoords = snake.GetCoordinates();
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

void
TextView::DrawFrame() const
{
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


//--------------------BASIC DRAWING--------------------

void
TextView::AddProperties(enum TextView::Color fg, enum TextView::Color bg) const
{
        printf("\e[%d;%dm", fg + CSI_fg_color_code, bg + CSI_bg_color_code);
        fflush(stdout);
}

void
TextView::HLine(int x0, int y0, int len, const char sym) const
{
    for(int i = x0; i < x0 + len; i++)
        Symbol({i, y0}, sym);
}

void
TextView::VLine(int x0, int y0, int len, const char sym) const
{
    for(int i = y0; i < y0 + len; i++)
        Symbol({x0, i}, sym);
}

void
TextView::Symbol(Coordinate coord, const char sym) const
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
TextView::TermiosPropsOn() const
{
    struct termios* termis_attr = TermiosAttrs();
    tcgetattr(STDIN_FILENO, termis_attr);

    struct termios attr_raw = *termis_attr;
    cfmakeraw(&attr_raw);
    
    /*attr_raw.c_cflag|= VINTR;
    attr_raw.c_iflag|= VINTR;
    attr_raw.c_ispeed|= VINTR;
    attr_raw.c_lflag|= VINTR;
    attr_raw.c_oflag|= VINTR;
    attr_raw.c_ospeed|= VINTR;*/
    
    attr_raw.c_cc[8] = 1;
    attr_raw.c_cc[9] = 1;
    attr_raw.c_lflag |= ISIG;

    tcsetattr(STDIN_FILENO, TCSANOW , &attr_raw);
}

void
TextView::TermiosPropsOff() const
{
    tcsetattr(STDIN_FILENO, TCSANOW, TermiosAttrs());
}

//----------------------------------------//
