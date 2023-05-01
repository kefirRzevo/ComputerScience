#pragma once


#include "graphlib/math.hpp"
#include "graphlib/colors.hpp"

class Texture;

static const float PI = 3.1416;

class Config
{
    public:

//----------------------------------------//

    static Texture* defPluginScrollBox;
    static Texture* defPluginScrollBar;

    static Texture* defCloseBtnRelease;
    static Texture* defCloseBtnHover;
    static Texture* defCloseBtnPress;

    static Texture* defScrollBoxRelease;
    static Texture* defScrollBoxHover;
    static Texture* defScrollBoxPress;

    static Texture* defHorScrollBarTexture;
    static Texture* defVerScrollBarTexture;

    static Texture* defUpReleaseTexture;
    static Texture* defUpHoverTexture;
    static Texture* defUpPressTexture;
    static Texture* defDownReleaseTexture;
    static Texture* defDownHoverTexture;
    static Texture* defDownPressTexture;

    static Texture* defCanvasTexture;
    static Texture* defTexture;

    static Texture* defAPIReleaseTexture;
    static Texture* defAPIHoverTexture;
    static Texture* defAPIPressTexture;
    static Texture* defAPILabelTexture;

//----------------------------------------//

    static const int defCloseButtonWidth  = 30;
    static const int defCloseButtonHeight = 30;

    static const int defIconWidth  = 30;
    static const int defIconHeight = 30;

    static const int defTextIconWidth    = 100;
    static const int defMinTextIconWidth = 75;
    static const int defMaxTextIconWidth = 125;
    static const int defTextIconHeight = 30;
    
    static const int defCanvasWidth  = 150;
    static const int defMinCanvasWidth = 100;
    static const int defMaxCanvasWidth = 200;
    static const int defCanvasHeight = 150;
    static const int defMinCanvasHeight = 100;
    static const int defMaxCanvasHeight = 200;

//----------------------------------------//

    static const int defWindowWidth  = 720;
    static const int defWindowHeight = 720;
    
    static const int defFullTextureWidth  = 400;
    static const int defFullTextureHeight = 400;

    static const int defMargin = 2;
    static const int defBorder = 4;

    static const Color defColor = Antique_white;
    static const Color defBorderColor = Black_bean;

    static inline const char* defSoPluginDir = "build/plugins/";


//----------------------------------------//

    static const int   defTextSize = 24;
    static const int   defTextMaxCapacity = 12;
    static const Color defTextColor = Black;

    static inline const char* defTextFontPath = "res/TimesNewRoman.ttf";

//----------------------------------------//

    static const int defAPI_IconButtonWidth  = 50;
    static const int defAPI_IconButtonHeight = 50;

    static const int defAPI_TextButtonWidth  = 50;
    static const int defAPI_TextButtonHeight = 50;

    static const int defAPI_SliderWidth  = 100;
    static const int defAPI_SliderHeight = 30;
    static const int defAPI_SliderBoxWidth  = 30;
    static const int defAPI_SliderBoxHeight = 30;

    static const int defAPI_LabelWidth  = 100;
    static const int defAPI_LabelHeight = 30;

//----------------------------------------//

};
