#pragma once


#include <termios.h>

#include "view.hpp"


//----------------------------------------//

class TextView: public View
{
    private:

        bool frameFull;
        Size frameSize;
        Coordinate upLeftCorner;
        struct termios termis_attr;
        std::string results;

    public:

        enum Color
            {
                BLACK   = 0,
                RED     = 1,
                GREEN   = 2,
                BROWN   = 3,
                BLUE    = 4,
                MAGENTA = 5,
                CYAN    = 6,
                WHITE   = 7,
            };

        TextView();

        ~TextView();

        void RunLoop() override;

        void DrawRabbits() const;

        void DrawSnakes() const;

        void DrawFrame();

    private:

        void RedrawRabbits() const;

        void RedrawSnakes() const;

        void AddProperties(enum TextView::Color fg, 
                           enum TextView::Color bg) const;

        void HLine(int x0, int y0, int len, int sym) const;

        void VLine(int x0, int y0, int len, int sym) const;

        void Symbol(Coordinate coord, int sym) const;

        void String(int x, int y, const char* string) const;

        void Clear() const;


        void CarretOff() const;

        void CarretOn() const;


        void TermiosPropsOn();

        void TermiosPropsOff();


        void PollOnKey();

        void UpdateResults();

        void UpdateWindowSize() override;
};

//----------------------------------------//
