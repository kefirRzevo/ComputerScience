#pragma once

#include <SFML/Graphics.hpp>

#include "view.hpp"

//----------------------------------------//

class GuiView : public View {
private:
  sf::RenderWindow sfWindow;
  sf::Font sfFont;
  sf::Texture sfSnakeTexture;
  sf::Texture sfRabbitTexture;

public:
  GuiView();

  ~GuiView();

  void RunLoop() override;

  void DrawRabbits();

  void DrawSnakes();

  void DrawFrame();

  void DrawResults();

private:
  sf::Color GetStyleColor(int snakeStyle) const;
};

//----------------------------------------//
