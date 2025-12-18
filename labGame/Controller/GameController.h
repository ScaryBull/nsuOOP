#ifndef LABGAME_GAMECONTROLLER_H
#define LABGAME_GAMECONTROLLER_H

#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <vector>
#include <optional>
#include <memory>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include "../view/GameView.h"
#include "../model/include/GameModel.h"
#include "../model/include/ItemFactories.h"
#include "../model/include/AlchemyLevel.h"
#include "../model/include/Amulet.h"
#include "../model/include/Item.h"


class GameController {
private:
  GameModel model;
  GameView view;

  bool showPotion = false;
  bool showShop = false;
  bool showOrders = false;
  bool showTrash = false;
  bool showMenuOverlay = false;
  bool amuletBaseSelected = false;
  bool starSelected = false;
  bool showBlackScreen = false;
  bool showSecretWindow = false;

  std::vector<int> selectedPotionsForStar;
  std::vector<int> selectedIngredients;
  std::vector<int> selectedPotionsForAmulet;
  std::vector<std::shared_ptr<Item>> itemCells;
  std::vector<GameView::IngredientSlot> allSlots;


  int mixIngredients(const std::vector<int>& ingredients);
  void handleShelfClick(float mouseX, float mouseY);
  void handleCauldronClick(float mouseX, float mouseY);
  void handleCauldronMix();
  void handleShopClick(float mouseX, float mouseY);
  void handleShopBuy(float mouseX, float mouseY);
  void handleOrdersButtonClick(float mouseX, float mouseY);
  void handleOrderClick(float mouseX, float mouseY);
  void handleTrashButtonClick(float mouseX, float mouseY);
  void handleTrashPotionClick(float mouseX, float mouseY);
  void handleBlackScreenClick(float mouseX, float mouseY, sf::RenderWindow& window);
  void refundSelections();

public:
  GameController();
  void handleEvents(sf::RenderWindow& window);
  void draw(sf::RenderWindow& window);
};

#endif