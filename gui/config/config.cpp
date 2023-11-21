#include "config.hpp"
#include "../graphlib/graphlib.hpp"

Texture *Config::defPluginScrollBox{
    TextureManager::Get()->GetTexture("res/scrollbox.bmp")};
Texture *Config::defPluginScrollBar{
    TextureManager::Get()->GetTexture("res/scrollbar.bmp")};

Texture *Config::defCloseBtnRelease{
    TextureManager::Get()->GetTexture("res/btns.jpg", {40, 0, 120, 120})};
Texture *Config::defCloseBtnHover{
    TextureManager::Get()->GetTexture("res/btns.jpg", {40, 120, 120, 120})};
Texture *Config::defCloseBtnPress{
    TextureManager::Get()->GetTexture("res/btns.jpg", {40, 240, 120, 120})};

Texture *Config::defScrollBoxRelease{
    TextureManager::Get()->GetTexture(Battleship_grey)};
Texture *Config::defScrollBoxHover{
    TextureManager::Get()->GetTexture(Black_coral)};
Texture *Config::defScrollBoxPress{
    TextureManager::Get()->GetTexture(Black_olive)};

Texture *Config::defHorScrollBarTexture{
    TextureManager::Get()->GetTexture(Bisque)};
Texture *Config::defVerScrollBarTexture{
    TextureManager::Get()->GetTexture(Bisque)};

Texture *Config::defUpReleaseTexture{
    TextureManager::Get()->GetTexture(Chocolat_traditional)};
Texture *Config::defUpHoverTexture{
    TextureManager::Get()->GetTexture(Brown_sugar)};
Texture *Config::defUpPressTexture{TextureManager::Get()->GetTexture(Coffee)};
Texture *Config::defDownReleaseTexture{
    TextureManager::Get()->GetTexture(Champagne_pink)};
Texture *Config::defDownHoverTexture{
    TextureManager::Get()->GetTexture(Antique_white)};
Texture *Config::defDownPressTexture{
    TextureManager::Get()->GetTexture(Apricot)};

Texture *Config::defNotFound{
    TextureManager::Get()->GetTexture("res/not_found.bmp")};
Texture *Config::defCanvasTexture{
    TextureManager::Get()->GetTexture(Blanched_almond)};
Texture *Config::defTexture{
    TextureManager::Get()->GetTexture("res/background.png")};

Texture *Config::defAPIReleaseTexture{
    TextureManager::Get()->GetTexture(Air_superiority_blue)};
Texture *Config::defAPIHoverTexture{
    TextureManager::Get()->GetTexture(Ash_gray)};
Texture *Config::defAPIPressTexture{
    TextureManager::Get()->GetTexture(Blue_yonder)};

Texture *Config::defAPILabelTexture{
    TextureManager::Get()->GetTexture(Blue_bell)};
