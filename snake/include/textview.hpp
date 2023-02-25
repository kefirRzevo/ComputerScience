#pragma once

#include "view.hpp"


class TextView: public View
{
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

        ~TextView()
            {
                signal(SIGWINCH, SIG_DFL);
                signal(SIGINT, SIG_DFL);
                CarretOn();
                DeleteProperties();
            }

        void Draw() const override
            {
                Clear();
                DrawRabbits();
                DrawSnakes();
                GetModel()->Update();
                DrawFrame();
            }

        size GetWindowSize() const override
            {
                return windowSize;
            }

        bool GetStatus() const override
            {
                return status;
            }

    private:
        size windowSize;
        bool status;

        void UpdateWindowSize() override;

        void UpdateStatus() override
            {
                status = !status;
            }

        void DrawRabbits() const;

        void DrawSnakes() const;

        void DrawFrame() const;

        Coordinate ModelCoordToView(const Coordinate& modelCoord) const;

        void AddProperties(enum TextView::Color fg, enum TextView::Color bg) const;

        void DeleteProperties() const
            {
                printf("\e[0m");
            }

        void HLine(int x0, int y0, int len, const char sym) const
            {
                for(int i = x0; i < x0 + len; i++)
                    Symbol({i, y0}, sym);
            }

        void VLine(int x0, int y0, int len, const char sym) const
            {
                for(int i = y0; i < y0 + len; i++)
                    Symbol({x0, i}, sym);
            }

        void Symbol(Coordinate coord, const char sym) const
            {
                printf("\e[%d;%dH", coord.second, coord.first);
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
