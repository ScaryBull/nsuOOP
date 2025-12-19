#ifndef LABGAME_GAMEVIEW_H
#define LABGAME_GAMEVIEW_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <memory>
#include <cmath>
#include <algorithm>
#include "../model/include/GameModel.h"
#include "../model/include/Potion.h"


class GameView {
public:
  struct IngredientSlot {
    float x, y;
    int index;
  };

private:
  sf::Texture backgroundTexture;
  sf::Texture shelfTexture;
  sf::Texture ingredientTextures[6];
  sf::Texture cauldronTexture;
  sf::Texture potionTexture;
  sf::Texture mixButtonTexture;
  sf::Texture potionResultTexture;
  sf::Texture amuletResultTexture;
  sf::Texture shopButtonTexture;
  sf::Texture ordersButtonTexture;
  sf::Texture trashButtonTexture;
  sf::Texture menuButtonTexture;
  sf::Texture level2Texture;
  sf::Texture level3Texture;
  sf::Texture amuletTexture;
  sf::Texture secretTexture;
  sf::Texture starTexture;

  sf::Font font;

  void drawShelves(sf::RenderWindow& window, const GameModel& model, std::vector<IngredientSlot>& allSlots, bool showPotion, bool amuletBaseSelected, bool starSelected);
  void drawShelfHook(sf::RenderWindow& window, const GameModel& model, bool showPotion, bool amuletBaseSelected);
  void drawButtons(sf::RenderWindow& window);
  void drawPotionWindow(sf::RenderWindow& window, const std::vector<int>& selectedIngredients, const std::vector<std::shared_ptr<Item>>& itemCells, const std::vector<int>& selectedPotionsForAmulet, const std::vector<int>& selectedPotionsForStar, bool amuletBaseSelected, bool starSelected);
  void drawPotionSlots(sf::RenderWindow& window, const std::vector<std::shared_ptr<Item>>& itemCells, const std::vector<int>& selectedPotionsForAmulet, const std::vector<int>& selectedPotionsForStar);
  void drawShopWindow(sf::RenderWindow& window, const GameModel& model);
  void drawTrashWindow(sf::RenderWindow& window, const std::vector<std::shared_ptr<Item>>& itemCells);
  void drawOrdersWindow(sf::RenderWindow& window, const GameModel& model);
  void drawTopBar(sf::RenderWindow& window, const GameModel& model);
  void drawSecretModal(sf::RenderWindow& window);
  void drawBlackScreenOverlay(sf::RenderWindow& window, bool showBlackScreen);
  bool loadTextureFromFile(sf::Texture& tex, const std::string& path);

  sf::Sprite makeSprite(const sf::Texture& tex, float x, float y, float targetWidth = 0.0f, float targetHeight = 0.0f);
  sf::Text makeText(const std::string& str, unsigned int charSize, sf::Color color, float x, float y, bool center = false);
  sf::RectangleShape makeRect(float width, float height, sf::Color fill, sf::Color outline = sf::Color::Transparent, float outlineThickness = 0.0f, float x = 0.0f, float y = 0.0f);

public:
  static constexpr float WINDOW_WIDTH = 1024.0f;
  static constexpr float WINDOW_HEIGHT = 576.0f;

  static constexpr float SHELF_WIDTH = 256.0f;
  static constexpr float SHELF_HEIGHT = 64.0f;
  static constexpr float SHELF_SPACING = 120.0f;
  static constexpr float SHELF_START_Y = 100.0f;
  static constexpr float INGREDIENT_SIZE = 100.0f;
  static constexpr float POTION_SIZE = 300.0f;

  static constexpr int POTION_CELL_SIZE = 80;
  static constexpr int POTION_CELL_PADDING = 20;

  static constexpr float CAULDRON_BUTTON_SIZE = 80.0f;
  static constexpr float CAULDRON_X = WINDOW_WIDTH - CAULDRON_BUTTON_SIZE - 20;
  static constexpr float CAULDRON_Y = WINDOW_HEIGHT - CAULDRON_BUTTON_SIZE - 20;
  static constexpr float MIX_BUTTON_SIZE = 60;

  static constexpr float SHOP_BUTTON_SIZE = 80.0f;
  static constexpr int SHOP_BUTTON_X = CAULDRON_X - SHOP_BUTTON_SIZE - 10;
  static constexpr int SHOP_BUTTON_Y = CAULDRON_Y;
  
  static constexpr float ORDERS_BUTTON_SIZE = 80.0f;
  static constexpr int ORDERS_X = SHOP_BUTTON_X - ORDERS_BUTTON_SIZE - 10;
  static constexpr int ORDERS_Y = CAULDRON_Y;
  
  static constexpr float TRASH_BUTTON_SIZE = 80.0f;
  static constexpr int TRASH_BUTTON_X = ORDERS_X - TRASH_BUTTON_SIZE - 10;
  static constexpr int TRASH_BUTTON_Y = CAULDRON_Y;
  static constexpr float TRASH_WIDTH = 600.0f;
  static constexpr float TRASH_HEIGHT = 200.0f;
  static constexpr float TRASH_X = (WINDOW_WIDTH - TRASH_WIDTH) / 2.0f;
  static constexpr float TRASH_Y = (WINDOW_HEIGHT - TRASH_HEIGHT) / 2.0f;

  static constexpr float MENU_BUTTON_SIZE = 50.0f;
  static constexpr float MENU_BUTTON_X = WINDOW_WIDTH - MENU_BUTTON_SIZE - 10.0f;
  static constexpr float MENU_BUTTON_Y = 10.0f;

  static constexpr float BTN_WIGHT = 160.0f;
  static constexpr float BTN_HEIGHT = 50.0f;
  static constexpr float BTN_Y = WINDOW_HEIGHT / 2.0f;
  static constexpr float SPACING = 20.0f;

  static constexpr float AMULET_SCALE = (INGREDIENT_SIZE / 50.0f) * 1.5f;
  static constexpr float AMULET_WIDTH = 20.0f * AMULET_SCALE;
  static constexpr float AMULET_HEIGHT = 50.0f * AMULET_SCALE;

  static constexpr int SHOP_CELL_SIZE = 70;
  static constexpr int COLS = 5;
  static constexpr int ROWS = 2;
  static constexpr int SHOP_WIDTH = COLS * SHOP_CELL_SIZE + (COLS - 1) * 8 + 30;
  static constexpr int SHOP_HEIGHT = ROWS * SHOP_CELL_SIZE + (ROWS - 1) * 8 + 70;
  static constexpr float SHOP_X = (WINDOW_WIDTH - SHOP_WIDTH) / 2.0f;
  static constexpr float SHOP_Y = (WINDOW_HEIGHT - SHOP_HEIGHT) / 2.0f;

  static constexpr float RADIUS = 80.0f;
  static constexpr int INGREDIENT_PER_POW = 2;
  static constexpr float INGREDIENT_SCALE = 0.8f;
  static constexpr float SCALE_SIZE = INGREDIENT_SIZE * INGREDIENT_SCALE;
  static constexpr float ELEMENT_WIDTH = 460.0f;
  static constexpr float ELEMENT_HEIGHT = 40.0f;
  static constexpr float ELEMENT_PADDING = 15.0f;

  static constexpr float PANEL_WIDTH = 500.0f;
  static constexpr float PANEL_HEIGHT = 300.0f;
  static constexpr float PANEL_X = (WINDOW_WIDTH - PANEL_WIDTH) / 2.0f;
  static constexpr float PANEL_Y = (WINDOW_HEIGHT - PANEL_HEIGHT) / 2.0f;

  static constexpr float LEVEL_SIZE = 80.0f;
  static constexpr float LEVEL_PADDING = 5.0f;
  static constexpr float LEVEL_Y = 10.0f;
  static constexpr float SECRET_Y = 10.0f;

  GameView();
  void draw(sf::RenderWindow& window, const GameModel& model, bool showPotion, bool showShop, bool showOrders, bool showTrash,
    const std::vector<int>& selectedIngredients, const std::vector<std::shared_ptr<Item>>& itemCells, std::vector<IngredientSlot>& allSlots,
    const std::vector<int>& selectedPotionsForStar, const std::vector<int>& selectedPotionsForAmulet,
    bool showSecretWindow = false, bool showBlackScreen = false, bool amuletBaseSelected = false, bool starSelected = false, bool showMenuOverlay = false);
};

#endif