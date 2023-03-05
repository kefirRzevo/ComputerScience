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

        ~TextView();

        void Run() override;

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

        void UpdateStatus() override;

        void DrawRabbits() const;

        void DrawSnakes() const;

        void DrawFrame() const;

        void PollOnKey() const;

        Coordinate ModelCoordToView(const Coordinate& modelCoord) const;

        void AddProperties(enum TextView::Color fg, enum TextView::Color bg) const;

        void HLine(int x0, int y0, int len, const char sym) const;

        void VLine(int x0, int y0, int len, const char sym) const;

        void Symbol(Coordinate coord, const char sym) const;

        void String(int x, int y, const std::string& string) const;

        void Clear() const;

        void CarretOff() const;

        void CarretOn() const;

        void TermiosPropsOn() const;

        void TermiosPropsOff() const;
};
