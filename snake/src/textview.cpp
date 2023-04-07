#include <sys/ioctl.h>
#include <sys/time.h> 
#include <termios.h>
#include <csignal>
#include <poll.h>
#include <cmath>

#include "../include/textview.hpp"
#include "../include/config.hpp"


Color Black    = 0;
Color Red      = 1;
Color Green    = 2;
Color Yellow   = 3;
Color Blue     = 4;
Color Magennta = 5;
Color Cyan     = 6;
Color White    = 7;

Color
Brighten(Color col);

Color
FromInt(int number);

Color
FromRGB(int r, int g, int b);

void 
SigHandler(int signum);

//----------------------------------------//

Color
Brighten(Color col)
{
    assert(col >= 0 && col <= 7);
    return col + 8;
}

//----------------------------------------//

Color
FromInt(int number)
{
    assert(number >=0 && number <= 255);
    return number;
}

//----------------------------------------//

Color
FromRGB(int r, int g, int b)
{
    assert(r >= 0 && g >= 0 && b >=0 && r <=5 && g <= 5 && b <= 5);
    return 16 + 36 * r + 6 * g + b;
}

//----------------------------------------//

KeyCode
fromTextView(int textKey)
{
    if(textKey == 'q' || textKey == 'Q')
        return keyQuit;
    else if(textKey == 'w' || textKey == 'W')
        return keyW;
    else if(textKey == 'a' || textKey == 'A')
        return keyA;
    else if(textKey == 's' || textKey == 'S')
        return keyS;
    else if(textKey == 'd' || textKey == 'D')
        return keyD;

    return keyUnknown;
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
        if(dynamic_cast<TextView* >(view))
            static_cast<TextView* >(view)->UpdateWindowSize();
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

KeyCode
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
        if(symbol != '\033')
        {
            return fromTextView(symbol);
        }
        else
        {
            read(STDIN_FILENO, &symbol, 1);
            if(symbol != '[')
                return keyUnknown;
            read(STDIN_FILENO, &symbol, 1);
            if(symbol == 'A')
                return keyArrowU;
            else if(symbol == 'B')
                return keyArrowD;
            else if(symbol == 'C')
                return keyArrowR;
            else if(symbol == 'D')
                return keyArrowL;
        }
    }

    return noKey;
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
            Clear(FromInt(141));
            DrawFrame();
            DrawResults();
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
        SetRabbitStyleColors(rabbit->GetStyleType());
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

        SetSnakeStyleColors(snake->GetStyleType());

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

    SetColor(FromInt(54), FromInt(27));
    HLine(cornerUpLeftX, cornerUpLeftY, frameWidth, '=');
    HLine(cornerUpLeftX, cornerUpLeftY + frameHeight - 1, frameWidth, '=');
    VLine(cornerUpLeftX, cornerUpLeftY, frameHeight, '#');
    VLine(cornerUpLeftX + frameWidth - 1, cornerUpLeftY, frameHeight, '#');

    frameSize.first  = frameWidth;
    frameSize.second = frameHeight;

    upLeftCorner.first  = cornerUpLeftX;
    upLeftCorner.second = cornerUpLeftY;
}

//----------------------------------------//

void
TextView::DrawResults() const
{
    int sumIndent = upLeftCorner.first;
    for(const auto& snake: GetModel()->snakes)
    {
        std::string playerResult;
        playerResult += "Player";
        playerResult += std::to_string(GetModel()->GetSnakeIndex(snake));
        playerResult += " ";
        playerResult += std::to_string(snake->GetScore());
        if(!snake->IsAlive())
            playerResult += "(Died)";
        playerResult += ";";
        
        SetSnakeStyleColors(snake->GetStyleType());
        String(sumIndent, upLeftCorner.second / 2, playerResult.c_str());
        sumIndent += playerResult.size();
        SetColor(-1, FromInt(141));
        Symbol({sumIndent++, upLeftCorner.second / 2}, ' ');
    }
}

//----------------------------------------//

void
TextView::SetSnakeStyleColors(int snakeStyle) const
{
    SetColor(FromInt(9 + snakeStyle), FromInt(1 + snakeStyle));
}

//----------------------------------------//

void
TextView::SetRabbitStyleColors(int rabbitStyle) const
{
    SetColor(FromInt(17 + 7 * rabbitStyle), FromInt(17 + 9 * rabbitStyle));
}

//----------------------------------------//

void
TextView::SetColor(Color fg, Color bg) const
{
    if(fg >= 0)
        printf("\e[38;5;%dm", fg);
    if(bg >= 0)
        printf("\e[48;5;%dm", bg);
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
TextView::Clear(Color bg) const
{
    SetColor(-1, bg);
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
