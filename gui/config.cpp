#include "config.hpp"
#include "graphlib/graphlib.hpp"

Texture*
Config::pluginScrollBox{TextureManager::Get()->GetTexture("res/scrollbox.bmp")};
Texture*
Config::pluginScrollBar{TextureManager::Get()->GetTexture("res/scrollbar.bmp")};

Texture*
Config::defCloseBtnRelease{TextureManager::Get()->GetTexture("res/btns.jpg", {40, 0, 120, 120})};
Texture*
Config::defCloseBtnHover{TextureManager::Get()->GetTexture("res/btns.jpg", {40, 120, 120, 120})};
Texture*
Config::defCloseBtnPress{TextureManager::Get()->GetTexture("res/btns.jpg", {40, 240, 120, 120})};

Texture*
Config::defScrollBoxRelease{TextureManager::Get()->GetTexture(0x84'84'82'FF)}; //Battleship grey
Texture*
Config::defScrollBoxHover{TextureManager::Get()->GetTexture(0x54'62'6F'FF)}; //Black coral
Texture*
Config::defScrollBoxPress{TextureManager::Get()->GetTexture(0x3B'3C'36'FF)}; //Black olive

Texture*
Config::defHorScrollBarTexture{TextureManager::Get()->GetTexture(0xFF'E4'C4'FF)}; //Bisque
Texture*
Config::defVerScrollBarTexture{TextureManager::Get()->GetTexture(0xFF'E4'C4'FF)}; //Bisque
Texture*
Config::defIconTexture{TextureManager::Get()->GetTexture(0xFF'EB'CD'FF)}; //Blanched almond
