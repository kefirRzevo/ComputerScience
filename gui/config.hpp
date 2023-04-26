#pragma once

#include "graphlib/math.hpp"

enum class HorAlign;
enum class VerAlign;

static const double PI = 3.1416;

class Config
{
    public:

//----------------------------------------//

        static const int defWidth = 50;
        static const int minWidth = 25;
        static const int maxWidth = 75;
        static const int defHeight = 50;
        static const int minHeight = 25;
        static const int maxHeight = 75;


        static const unsigned int windowWidth  = 720;
        static const unsigned int windowHeight = 720;

        static const int   defMargin = 2;
        static const int   defBorder = 4;
        static const Color defColor  = 0xFA'EB'D7'FF; //Antique white

        static const Color defBorderColor = 0x3D'0C'02'FF; //Black bean
        static const Color defPanelColor  = 0xBF'AF'B2'FF; //Black shadows

//----------------------------------------//

        static const int defHorScrollBoxHeight = 30;
        static const int defHorScrollBoxWidth = 30;

        static const int defHorScrollBarHeight = 30;
        static const int defHorScrollBarWidth = 120;
        static const int minHorScrollBarWidth = 60;
        static const int maxHorScrollBarWidth = 180;

        static inline const char* defScrollBoxPath = "res/scrollbox.bmp";
        static inline const char* defScrollBarPath = "res/scrollbar.bmp";

//----------------------------------------//

        static const int defIconWidth = 60;
        static const int minIconWidth = 45;
        static const int maxIconWidth = 75;
        static const int defIconHeight = 30;
        static const int minIconHeight = 25;
        static const int maxIconHeight = 35;

        static const int defTextSize        = 24;
        static const int defTextMaxCapacity = 12;

        static const Color defTextColor = 0x00'00'00'FF; //Black
        static const Color defIconColor = 0xFB'CE'B1'FF; //Apricot

        static inline const char* defTextFontPath    = "res/TimesNewRoman.ttf";
        static inline const char* defIconTexturePath = "res/button.png";

//----------------------------------------//

};
