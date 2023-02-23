#pragma once

#include <iostream>
#include <cstdlib>
#include <unistd.h>

#include "model.hpp"

using pair = std::pair<int, int>;

class View
{
    protected:
        pair windowSize;
        bool status;

    public:
        View() = default;

        virtual ~View()
            {}

	    static View* Get(const std::string& mode = "text");

        pair GetWindowSize() const
            {
                return windowSize;
            }

        bool GetStatus() const
            {
                return status;
            }

        virtual void  Draw() const = 0;
        virtual void  Redraw() const = 0;

        friend void SigHandler(int signum);

    private:
        virtual void UpdateWindowSize() = 0;
};


class GuiView: public View
{
    public:
        GuiView()
            {};

        ~GuiView()
            {}

        void Draw() const override
            {
                std::cout << "Not implemented" << std::endl;
            }
    
        void Redraw() const override
            {
                std::cout << "Not implemented" << std::endl;
            }

    private:
        void UpdateWindowSize() override
            {
                std::cout << "Not implemented" << std::endl;
            }
};

class TextView: public View
{
    public:
        TextView();

        ~TextView()
            {
                signal(SIGWINCH, SIG_DFL);
                signal(SIGINT, SIG_DFL);
                CarretOn();
            }

        void Draw() const override
            {
                pair size = GetWindowSize();
                int width = size.first;
                int height = size.second;
                //std::cout << width << " " << height << std::endl;
                AddProperties(RED, WHITE);
                HLine(1, 1, width, '=');
                HLine(1, height, width, '=');
                VLine(1, 1, height, '#');
                VLine(width, 1, height, '#');
                String(10, 1, "Hello world!");

                fflush(stdout);
            }

        virtual void  Redraw() const override
            {
                Clear();
                Draw();
            }

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
        void UpdateWindowSize() override;

        void AddProperties(enum TextView::Color fg, enum TextView::Color bg) const;

        void DeleteProperties()
            {
                printf("\e[0m");
            }

        void HLine(int x0, int y0, int len, const char sym) const
            {
                for(int i = x0; i < x0 + len; i++)
                    Symbol(i, y0, sym);
            }

        void VLine(int x0, int y0, int len, const char sym) const
            {
                for(int i = y0; i < y0 + len; i++)
                    Symbol(x0, i, sym);
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

        void Clear() const
            {
                printf("\e[2J");
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
