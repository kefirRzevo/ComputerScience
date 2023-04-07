#pragma once


#include <termios.h>

#include "view.hpp"

using Color = int;

//----------------------------------------//

class TextView: public View
{
    private:

        Size windowSize;
        Size frameSize;
        Coordinate upLeftCorner;
        bool timeToDraw;

        struct termios termis_attr;

    public:

        TextView();

        ~TextView();

        void RunLoop() override;

        void DrawRabbits() const;

        void DrawSnakes() const;

        void DrawResults() const;

        void DrawFrame();

    private:

        void SetSnakeStyleColors(int snakeStyle) const;

        void SetRabbitStyleColors(int rabbitStyle) const;

        void SetColor(Color fg, Color bg) const;

        void HLine(int x0, int y0, int len, int sym) const;

        void VLine(int x0, int y0, int len, int sym) const;

        void Symbol(Coordinate coord, int sym) const;

        void String(int x, int y, const char* string) const;

        void Clear(Color bg = -1) const;


        void CarretOff() const;

        void CarretOn() const;


        void TermiosPropsOn();

        void TermiosPropsOff();

        
        bool IsInFrame(const Coordinate& point) const;

        KeyCode GetPolledKey() const;

        void OnTimer();

        void UpdateWindowSize();

        friend void SigHandler(int signum);
};

//----------------------------------------//
