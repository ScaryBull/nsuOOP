#include <SFML/Graphics.hpp>
#include "controller/GameController.h"

int main() {
  const int WINDOW_WIDTH = 1024;
  const int WINDOW_HEIGHT = 576;

  sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Magic Laboratory");
  window.setFramerateLimit(60);

  GameController controller;
  while (window.isOpen()) {
    controller.handleEvents(window);
    controller.draw(window);
    window.display();
  }

  return 0;
}