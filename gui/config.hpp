#pragma once

#include "graphlib/math.hpp"

enum class HorAlign;
enum class VerAlign;

static const double PI = 3.1416;

class Config
{
    public:

        static const int minWidth  = 25;
        static const int minHeight = 25;
        static const int maxWidth  = 75;
        static const int maxHeight = 75;

        static const unsigned int windowWidth  = 720;
        static const unsigned int windowHeight = 720;

        static const int   defMargin = 2;
        static const int   defBorder = 4;
        static const Color defBorderColor = Black;

        static constexpr char* defFontPath    = "res/TimesNewRoman.ttf";
        static constexpr char* defTexturePath = "res/button.png";

        static const int   defTextSize  = 24;
        static const Color defTextColor = Black;
};
