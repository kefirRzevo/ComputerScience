#pragma once


#include "graphlib/math.hpp"
#include "graphlib/colors.hpp"

class Texture;

static const float PI = 3.1416;

class Config
{
    public:

//----------------------------------------//

        static Texture* pluginScrollBox;
        static Texture* pluginScrollBar;

        static Texture* defCloseBtnRelease;
        static Texture* defCloseBtnHover;
        static Texture* defCloseBtnPress;

        static Texture* defScrollBoxRelease;
        static Texture* defScrollBoxHover;
        static Texture* defScrollBoxPress;

        static Texture* defHorScrollBarTexture;
        static Texture* defVerScrollBarTexture;
        static Texture* defIconTexture;

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
        static const Color defColor  = Antique_white;

        static const Color defBorderColor = Black_bean;
        static const Color defPanelColor  = Black_Shadows;
        static const Color defCanvasColor = Champagne_pink;

        static inline const char* defSoPluginDir = "build/plugins/";

//----------------------------------------//

        static const int defCloseBtnHeight = 30;
        static const int defCloseBtnWidth = 30;

//----------------------------------------//

        static const int defHorScrollBoxHeight = 30;
        static const int defHorScrollBoxWidth  = 30;

        static const int defHorScrollBarHeight = 30;
        static const int defHorScrollBarWidth = 150;
        static const int minHorScrollBarWidth = 100;
        static const int maxHorScrollBarWidth = 200;

//----------------------------------------//

        static const int defVerScrollBoxHeight = 30;
        static const int defVerScrollBoxWidth  = 30;

        static const int defVerScrollBarWidth = 30;
        static const int defVerScrollBarHeight = 150;
        static const int minVerScrollBarHeight = 100;
        static const int maxVerScrollBarHeight = 200;

//----------------------------------------//

        static const int defIconWidth = 150;
        static const int minIconWidth = 100;
        static const int maxIconWidth = 200;
        static const int defIconHeight = 30;
        static const int minIconHeight = 30;
        static const int maxIconHeight = 30;

        static const Color defIconColor = Apricot;

        static const int defTextSize = 24;
        static const int defTextMaxCapacity = 12;

        static const Color defTextColor = Black;
        static inline const char* defTextFontPath    = "res/TimesNewRoman.ttf";

//----------------------------------------//

};
