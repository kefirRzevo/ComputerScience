#pragma once

#include <iostream>
#include <cstdlib>
#include <unistd.h>

#include "vector.hpp"

class View
{
    private:
        iVec size;

    public:
        View()
            {};

        virtual ~View()
            {
                std::cout << "lala";
		        delete View::Get();
	        };

	    static  View* Get(const std::string& mode = "text");

        virtual void  Draw() const = 0;
        virtual iVec  GetWinSize() const = 0;
        virtual void  UpdateWinSize() = 0;
};


class GuiView: public View
{
    public:
        GuiView()
            {};

        ~GuiView()
            {};

        void Draw() const override
            {
                std::cout << "Not implemented" << std::endl;
            }

        iVec GetWinSize() const override
            {
                std::cout << "Not implemented" << std::endl;
                return iVec();
            }

        void  UpdateWinSize() override
            {

            }
};

class TextView: public View
{
    private:

        unsigned int width;
        unsigned int height;

    public:

        TextView();

        ~TextView()
            {
                CarretOn();
            }

        void Draw() const override
            {
                Cls();
                AddProperties(RED, BLUE);
                HLine(1, width, 1, '=');
                HLine(1, width-1, height, '=');
                VLine(1, 1, height, '#');
                VLine(width, 1, height, '#');
                String(10, 1, "Hello world!");
                std::cin.get();
            }

        iVec GetWinSize() const override
            {
                return iVec(width, height);
            }

        void UpdateWinSize() override;

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

    private:
    
        void DeleteProperties()
            {
                printf("\e[0m");
            }

        void AddProperties(enum TextView::Color fg, enum TextView::Color bg) const;

        void HLine(int x1, int x2, int y, const char sym) const
            {
                for(int x = x1; x <= x2; x++)
                    Symbol(x, y, sym);
            }

        void VLine(int x, int y1, int y2, const char sym) const
            {
                for(int y = y1; y <=y2; y++)
                    Symbol(x, y, sym);
            }

        void Symbol(int x, int y, const char sym) const
            {
                printf("\e[%d;%dH", y, x);
                printf("%c", sym);
            }

        void String(int x, int y, const std::string& string) const
            {
                printf("\e[%d;%dH", y, x);
                printf("%s", string.c_str());
            }

        void Cls() const
            {
                printf("\e[2J");
                fflush(stdout);
            }

        void CarretOff() const
            {
                printf("\e[?25l");
            }

        void CarretOn() const
            {
                printf("\e[?25h");
            }
};
