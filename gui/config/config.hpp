#pragma once

#include "../graphlib/colors.hpp"
#include "../graphlib/math.hpp"

class Texture;

static const float PI = 3.1416;

class Config {
public:
  //----------------------------------------//

  static Texture *defPluginScrollBox;
  static Texture *defPluginScrollBar;

  static Texture *defCloseBtnRelease;
  static Texture *defCloseBtnHover;
  static Texture *defCloseBtnPress;

  static Texture *defScrollBoxRelease;
  static Texture *defScrollBoxHover;
  static Texture *defScrollBoxPress;

  static Texture *defHorScrollBarTexture;
  static Texture *defVerScrollBarTexture;

  static Texture *defUpReleaseTexture;
  static Texture *defUpHoverTexture;
  static Texture *defUpPressTexture;
  static Texture *defDownReleaseTexture;
  static Texture *defDownHoverTexture;
  static Texture *defDownPressTexture;

  static Texture *defNotFound;
  static Texture *defCanvasTexture;
  static Texture *defTexture;

  static Texture *defAPIReleaseTexture;
  static Texture *defAPIHoverTexture;
  static Texture *defAPIPressTexture;
  static Texture *defAPILabelTexture;

  //----------------------------------------//

  static const int defCloseButtonWidth = 50;
  static const int defCloseButtonHeight = 50;

  static const int defIconWidth = 50;
  static const int defIconHeight = 50;

  static const int defTextIconWidth = 200;
  static const int defMinTextIconWidth = 150;
  static const int defMaxTextIconWidth = 250;
  static const int defTextIconHeight = 50;

  static const int defCanvasWidth = 300;
  static const int defMinCanvasWidth = 200;
  static const int defMaxCanvasWidth = 400;
  static const int defCanvasHeight = 300;
  static const int defMinCanvasHeight = 200;
  static const int defMaxCanvasHeight = 400;

  //----------------------------------------//

  static const int defWindowWidth = 1440;
  static const int defWindowHeight = 1440;

  static const int defFullTextureWidth = 800;
  static const int defFullTextureHeight = 800;

  static const int defMargin = 4;
  static const int defBorder = 8;

  static const Color defColor = Antique_white;
  static const Color defBorderColor = Black_bean;

  static inline const char *defSoPluginDir = "build/plugins/";

  //----------------------------------------//

  static const int defTextSize = 25;
  static const int defTextMaxCapacity = 20;
  static const Color defTextColor = Black;

  static inline const char *defTextFontPath = "res/TimesNewRoman.ttf";

  //----------------------------------------//

  static const int defAPI_IconButtonWidth = 50;
  static const int defAPI_IconButtonHeight = 50;

  static const int defAPI_TextButtonWidth = 50;
  static const int defAPI_TextButtonHeight = 50;

  static const int defAPI_SliderWidth = 100;
  static const int defAPI_SliderHeight = 30;
  static const int defAPI_SliderBoxWidth = 30;
  static const int defAPI_SliderBoxHeight = 30;

  static const int defAPI_LabelWidth = 100;
  static const int defAPI_LabelHeight = 30;

  //----------------------------------------//
};
