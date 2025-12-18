#include "GameController.h"


GameController::GameController() : model(), view() {
  for (int i = 0; i < 6; ++i) {
    itemCells.push_back(nullptr);
  }
  starSelected = false;
  selectedPotionsForStar.clear();
  showBlackScreen = false;
}

int GameController::mixIngredients(const std::vector<int>& ingredients) {
  if (ingredients.empty())
    return -1;
  int result = 0;
  for (int idx : ingredients) {
    result = (result + idx) % 6;
  }
  return result;
}

void GameController::refundSelections() {
  for (int ingIdx : selectedIngredients) {
    model.setIngredientQuantity(ingIdx, model.getIngredientQuantity(ingIdx) + 1);
  }
  selectedIngredients.clear();

  if (amuletBaseSelected) {
    model.setAmuletBases(model.getAmuletBases() + 1);
    amuletBaseSelected = false;
  }

  if (starSelected) {
    model.addStar(1);
    starSelected = false;
  }

  selectedPotionsForAmulet.clear();
  selectedPotionsForStar.clear();
}

void GameController::handleShelfClick(float mouseX, float mouseY) {
  if (showPotion) {
    int maxIngredients = model.getMaxIngredients();

    if (model.isSecretPurchased()) {
      float shelfY = GameView::SHELF_START_Y + GameView::SHELF_SPACING;
      float starSize = GameView::INGREDIENT_SIZE;
      float starX = GameView::WINDOW_WIDTH - GameView::SHELF_WIDTH - 20.0f - starSize;
      float starY = shelfY - (GameView::SHELF_HEIGHT + GameView::INGREDIENT_SIZE) / 2.0f;
      sf::FloatRect starBounds(sf::Vector2f(starX, starY), sf::Vector2f(starSize, starSize));
      if (starBounds.contains(sf::Vector2f(mouseX, mouseY))) {
        if (!starSelected) {
          if (model.decrementStar()) {
            starSelected = true;
            if (amuletBaseSelected) {
              model.setAmuletBases(model.getAmuletBases() + 1);
              amuletBaseSelected = false;
              selectedPotionsForAmulet.clear();
            }
          }
        } else {
          model.addStar(1);
          starSelected = false;
          selectedPotionsForStar.clear();
        }
        return;
      }
    }

    if (model.hasLevel3() && (model.getAmuletBases() > 0 || amuletBaseSelected) && selectedIngredients.empty()) {
      float hookY = GameView::SHELF_START_Y - (GameView::SHELF_HEIGHT + GameView::INGREDIENT_SIZE) / 2.0f;
      sf::Texture hookTexture;
      float hookTexHeight = static_cast<float>(GameView::INGREDIENT_SIZE);
      if (hookTexture.loadFromFile("hook.png")) {
        hookTexHeight = static_cast<float>(hookTexture.getSize().y);
      }

      float hookScale = (GameView::INGREDIENT_SIZE / hookTexHeight) / 1.5f;
      float amuletX = GameView::SHELF_WIDTH + 20.0f;
      float amuletY = hookY + hookTexHeight * hookScale - 30.0f;

      sf::FloatRect amuletBounds(sf::Vector2f(amuletX, amuletY), sf::Vector2f(GameView::AMULET_WIDTH, GameView::AMULET_HEIGHT));
      if (amuletBounds.contains(sf::Vector2f(mouseX, mouseY))) {
        if (!amuletBaseSelected) {
          if (model.getAmuletBases() > 0) {
            amuletBaseSelected = true;
            model.decrementAmuletBase();
          }
        } else {
          amuletBaseSelected = false;
          model.setAmuletBases(model.getAmuletBases() + 1);
        }
        selectedPotionsForAmulet.clear();
        return;
      }
    }

    if (starSelected && !selectedPotionsForStar.empty()) {
      float buttonX = (GameView::WINDOW_WIDTH + GameView::POTION_SIZE) / 2.0f - GameView::MIX_BUTTON_SIZE - 10;
      float buttonY = (GameView::WINDOW_HEIGHT + GameView::POTION_SIZE) / 2.0f - GameView::MIX_BUTTON_SIZE - 10;
      if (mouseX >= buttonX && mouseX <= buttonX + GameView::MIX_BUTTON_SIZE && mouseY >= buttonY && mouseY <= buttonY + GameView::MIX_BUTTON_SIZE) {
        int firstSlotIdx = selectedPotionsForStar[0];
        if (itemCells[firstSlotIdx] != nullptr && itemCells[firstSlotIdx]->getType() == "potion") {
          const auto& props = itemCells[firstSlotIdx]->getProperties();
          if (static_cast<int>(props.size()) == 6) {
            itemCells[firstSlotIdx] = nullptr;
            showPotion = false;
            starSelected = false;
            selectedPotionsForStar.clear();
            showBlackScreen = true;
            return;
          }
        }
      }
    }

    if (amuletBaseSelected && !selectedPotionsForAmulet.empty()) {
      float buttonX = (GameView::WINDOW_WIDTH + GameView::POTION_SIZE) / 2.0f - GameView::MIX_BUTTON_SIZE - 10;
      float buttonY = (GameView::WINDOW_HEIGHT + GameView::POTION_SIZE) / 2.0f - GameView::MIX_BUTTON_SIZE - 10;
      sf::FloatRect buttonBounds(sf::Vector2f(buttonX, buttonY), sf::Vector2f(GameView::MIX_BUTTON_SIZE, GameView::MIX_BUTTON_SIZE));
      if (buttonBounds.contains(sf::Vector2f(mouseX, mouseY))) {
        std::vector<std::string> properties;
        std::string amuletName = "Amulet";
        
        for (int slotIdx : selectedPotionsForAmulet) {
          auto& potion = itemCells[slotIdx];
          std::vector<std::string> potionProps = potion->getProperties();
          properties.insert(properties.end(), potionProps.begin(), potionProps.end());
          amuletName += "_" + potion->getName();
        }

        std::ostringstream oss;
        oss << std::quoted(amuletName) << ' ' << 3.0 << ' ' << std::quoted(std::string("amulet_result.png")) << ' ' << properties.size();
        for (const auto &p : properties) oss << ' ' << std::quoted(p);
        std::istringstream iss(oss.str());
        std::shared_ptr<Item> amuletPtr;
        try {
          amuletPtr = ItemFactory::Instance().CreateObject(std::string("Amulet"));
        } catch (...) {
          amuletPtr = std::make_shared<Amulet>();
        }
        amuletPtr->load(iss);

        int firstSlotIdx = selectedPotionsForAmulet[0];
        for (int slotIdx : selectedPotionsForAmulet) {
          itemCells[slotIdx] = nullptr;
        }

        itemCells[firstSlotIdx] = amuletPtr;
        showPotion = false;
        amuletBaseSelected = false;
        selectedPotionsForAmulet.clear();
        return;
      }
    }

    if (amuletBaseSelected) {
      for (int i = 0; i < 6; ++i) {
        float cellX = GameView::POTION_CELL_PADDING + i * (GameView::POTION_CELL_SIZE + GameView::POTION_CELL_PADDING);
        float cellY = GameView::WINDOW_HEIGHT - GameView::POTION_CELL_SIZE - GameView::POTION_CELL_PADDING;

        sf::FloatRect slotBounds(sf::Vector2f(cellX, cellY), sf::Vector2f(GameView::POTION_CELL_SIZE, GameView::POTION_CELL_SIZE));
        
        if (slotBounds.contains(sf::Vector2f(mouseX, mouseY))) {
          if (itemCells[i] != nullptr && itemCells[i]->getType() == "potion") {
            auto it = std::find(selectedPotionsForAmulet.begin(), selectedPotionsForAmulet.end(), i);
            if (it != selectedPotionsForAmulet.end()) {
              selectedPotionsForAmulet.clear();
            } else {
              selectedPotionsForAmulet.clear();
              selectedPotionsForAmulet.push_back(i);
            }
            return;
          }
        }
      }
    }

    if (amuletBaseSelected)
      return;

    if (starSelected) {
      for (int i = 0; i < 6; ++i) {
        float cellX = GameView::POTION_CELL_PADDING + i * (GameView::POTION_CELL_SIZE + GameView::POTION_CELL_PADDING);
        float cellY = GameView::WINDOW_HEIGHT - GameView::POTION_CELL_SIZE - GameView::POTION_CELL_PADDING;

        sf::FloatRect slotBounds(sf::Vector2f(cellX, cellY), sf::Vector2f(GameView::POTION_CELL_SIZE, GameView::POTION_CELL_SIZE));

        if (slotBounds.contains(sf::Vector2f(mouseX, mouseY))) {
          if (itemCells[i] != nullptr && itemCells[i]->getType() == "potion") {
            auto it = std::find(selectedPotionsForStar.begin(), selectedPotionsForStar.end(), i);
            if (it != selectedPotionsForStar.end()) {
              selectedPotionsForStar.clear();
            } else {
              selectedPotionsForStar.clear();
              selectedPotionsForStar.push_back(i);
            }
            return;
          }
        }
      }
    }

    if (starSelected) {
      return;
    }
    
    for (const auto& slot : allSlots) {
      sf::FloatRect bounds(sf::Vector2f(slot.x, slot.y),
        sf::Vector2f(GameView::INGREDIENT_SIZE, GameView::INGREDIENT_SIZE)
        );
      if (bounds.contains(sf::Vector2f(mouseX, mouseY))) {
        int idx = slot.index;
        auto it = std::find(selectedIngredients.begin(), selectedIngredients.end(), idx);
        if (it != selectedIngredients.end()) {
          selectedIngredients.erase(it);
          model.setIngredientQuantity(idx, model.getIngredientQuantity(idx) + 1);
        }
        else if (model.getIngredientQuantity(idx) > 0 && selectedIngredients.size() < maxIngredients) {
          selectedIngredients.push_back(idx);
          model.decrementIngredient(idx);
        }
        return;
      }
    }

    if (selectedIngredients.size() >= 2) {
      float buttonX = (GameView::WINDOW_WIDTH + GameView::POTION_SIZE) / 2.0f - GameView::MIX_BUTTON_SIZE - 10;
      float buttonY = (GameView::WINDOW_HEIGHT + GameView::POTION_SIZE) / 2.0f - GameView::MIX_BUTTON_SIZE - 10;
      sf::FloatRect buttonBounds(sf::Vector2f(buttonX, buttonY), sf::Vector2f(GameView::MIX_BUTTON_SIZE, GameView::MIX_BUTTON_SIZE));
      if (buttonBounds.contains(sf::Vector2f(mouseX, mouseY))) {
        std::vector<std::string> properties;
        std::string potionName = "Potion_";
        
        for (int ingIdx : selectedIngredients) {
          const auto& ing = model.getIngredient(ingIdx);
          properties.push_back(ing->getProperties().front());
          potionName += ing->getName() + "_";
        }

        std::ostringstream oss;
        oss << std::quoted(potionName) << ' ' << 1.0 << ' ' << std::quoted(std::string("potion_result.png")) << ' ' << properties.size();
        for (const auto &p : properties) oss << ' ' << std::quoted(p);
        std::istringstream iss(oss.str());
        std::shared_ptr<Item> potionPtr;
        try {
          potionPtr = ItemFactory::Instance().CreateObject(std::string("Potion"));
        } catch (...) {
          potionPtr = std::make_shared<Potion>();
        }
        potionPtr->load(iss);
        model.addPotion(potionPtr);

        for (int i = 0; i < 6; ++i) {
          if (itemCells[i] == nullptr) {
            itemCells[i] = potionPtr;
            break;
          }
        }
        showPotion = false;
        selectedIngredients.clear();
        return;
      }
    }
  }
}

void GameController::handleCauldronClick(float mouseX, float mouseY) {
    if (mouseX >= GameView::CAULDRON_X && mouseX < GameView::CAULDRON_X + GameView::CAULDRON_BUTTON_SIZE &&
      mouseY >= GameView::CAULDRON_Y && mouseY <= GameView::CAULDRON_Y + GameView::CAULDRON_BUTTON_SIZE) {
      if (showSecretWindow) {
        showSecretWindow = false;
        showPotion = true;
        showShop = false;
        showOrders = false;
        showTrash = false;
        amuletBaseSelected = false;
        selectedPotionsForAmulet.clear();
        return;
      }

      if (showPotion) {
        refundSelections();
        showPotion = false;
      } else {
        showPotion = true;
        showShop = false;
        showOrders = false;
        showTrash = false;
        amuletBaseSelected = false;
        starSelected = false;
        selectedPotionsForAmulet.clear();
        selectedPotionsForStar.clear();
    }
  }
}

void GameController::handleShopClick(float mouseX, float mouseY) {
  if (mouseX >= GameView::SHOP_BUTTON_X && mouseX <= GameView::SHOP_BUTTON_X + GameView::SHOP_BUTTON_SIZE &&
    mouseY >= GameView::SHOP_BUTTON_Y && mouseY <= GameView::SHOP_BUTTON_Y + GameView::SHOP_BUTTON_SIZE) {
    if (showPotion) {
      refundSelections();
      showPotion = false;
    }
    if (showSecretWindow) {
      showSecretWindow = false;
      showShop = true;
      showOrders = false;
      showTrash = false;
      if (amuletBaseSelected) {
        model.setAmuletBases(model.getAmuletBases() + 1);
        amuletBaseSelected = false;
      }
      selectedPotionsForAmulet.clear();
        if (starSelected) {
          model.addStar(1);
          starSelected = false;
        }
        selectedPotionsForStar.clear();
      return;
    }

    showShop = !showShop;
    if (showShop) {
      showOrders = false;
      showTrash = false;
      if (amuletBaseSelected) {
        model.setAmuletBases(model.getAmuletBases() + 1);
        amuletBaseSelected = false;
      }
      if (starSelected) {
        model.addStar(1);
        starSelected = false;
      }
      selectedPotionsForStar.clear();
      selectedPotionsForAmulet.clear();
    }
  }
}

void GameController::handleOrdersButtonClick(float mouseX, float mouseY) {
  if (mouseX >= GameView::ORDERS_X && mouseX <= GameView::ORDERS_X + GameView::ORDERS_BUTTON_SIZE &&
    mouseY >= GameView::ORDERS_Y && mouseY <= GameView::ORDERS_Y + GameView::ORDERS_BUTTON_SIZE) {
    if (showPotion) {
      refundSelections();
      showPotion = false;
    }
    if (showSecretWindow) {
      showSecretWindow = false;
      showOrders = true;
      showShop = false;
      showTrash = false;
      if (amuletBaseSelected) {
        model.setAmuletBases(model.getAmuletBases() + 1);
        amuletBaseSelected = false;
      }
      selectedPotionsForAmulet.clear();
        if (starSelected) {
          model.addStar(1);
          starSelected = false;
        }
        selectedPotionsForStar.clear();
      return;
    }

    showOrders = !showOrders;
    if (showOrders) {
      showShop = false;
      showTrash = false;
      if (amuletBaseSelected) {
        model.setAmuletBases(model.getAmuletBases() + 1);
        amuletBaseSelected = false;
      }
      if (starSelected) {
        model.addStar(1);
        starSelected = false;
      }
      selectedPotionsForStar.clear();
      selectedPotionsForAmulet.clear();
    }
  }
}

void GameController::handleTrashButtonClick(float mouseX, float mouseY) {
  if (mouseX >= GameView::TRASH_BUTTON_X && mouseX <= GameView::TRASH_BUTTON_X + GameView::TRASH_BUTTON_SIZE &&
    mouseY >= GameView::TRASH_BUTTON_Y && mouseY <= GameView::TRASH_BUTTON_Y + GameView::TRASH_BUTTON_SIZE) {
    if (showPotion) {
      refundSelections();
      showPotion = false;
    }
    if (showSecretWindow) {
      showSecretWindow = false;
      showTrash = true;
      showShop = false;
      showOrders = false;
      if (amuletBaseSelected) {
        model.setAmuletBases(model.getAmuletBases() + 1);
        amuletBaseSelected = false;
      }
      selectedPotionsForAmulet.clear();
        if (starSelected) {
          model.addStar(1);
          starSelected = false;
        }
        selectedPotionsForStar.clear();
      return;
    }

    showTrash = !showTrash;
    if (showTrash) {
      showShop = false;
      showOrders = false;
      if (amuletBaseSelected) {
        model.setAmuletBases(model.getAmuletBases() + 1);
        amuletBaseSelected = false;
      }
      selectedPotionsForAmulet.clear();
    }
  }
}

void GameController::handleShopBuy(float mouseX, float mouseY) {
  for (int i = 0; i < 10; ++i) {
    int col = i % GameView::COLS;
    int row = i / GameView::COLS;
    float cellX = GameView::SHOP_X + 15 + col * (GameView::SHOP_CELL_SIZE + 8);
    float cellY = GameView::SHOP_Y + 15 + row * (GameView::SHOP_CELL_SIZE + 8);

    if (mouseX >= cellX && mouseX < cellX + GameView::SHOP_CELL_SIZE &&
      mouseY >= cellY && mouseY < cellY + GameView::SHOP_CELL_SIZE) {

      if (i < 6) {
        if (model.getGold() >= 5) {
          int currentQuantity = model.getIngredientQuantity(i);
          model.setIngredientQuantity(i, currentQuantity + 1);
          model.addGold(-5);
        }
      } else if (i == 6) {
        const auto& levels = model.getUnlockedLevels();
        bool hasLevel2 = false;
        for (const auto& level : levels) {
          if (level.getLevelNumber() == 2) {
            hasLevel2 = true;
            break;
          }
        }
        
        if (!hasLevel2 && model.getGold() >= 50) {
          AlchemyLevel level2(2, 5, "II.png");
          model.unlockLevel(level2);
          model.addGold(-50);
          model.regenerateOrdersForLevel();
        }
      } else if (i == 7) {
        const auto& levels = model.getUnlockedLevels();
        bool hasLevel3 = false;
        bool hasLevel2 = false;
        for (const auto& level : levels) {
          if (level.getLevelNumber() == 3) {
            hasLevel3 = true;
          }
          if (level.getLevelNumber() == 2) {
            hasLevel2 = true;
          }
        }
        
        if (hasLevel2 && !hasLevel3 && model.getGold() >= 100) {
          AlchemyLevel level3(3, 5, "III.png");
          model.unlockLevel(level3);
          model.addGold(-100);
          model.regenerateOrdersForLevel();
        }
      } else if (i == 8) {
        const auto& levels = model.getUnlockedLevels();
        bool hasLevel3 = false;
        for (const auto& level : levels) {
          if (level.getLevelNumber() == 3) {
            hasLevel3 = true;
            break;
          }
        }
        if (hasLevel3 && model.getGold() >= 50) {
          int currentBases = model.getAmuletBases();
          model.setAmuletBases(currentBases + 1);
          model.addGold(-50);
        }
      } else if (i == 9) {
        const auto& levels = model.getUnlockedLevels();
        bool hasLevel3 = false;
        for (const auto& level : levels) {
          if (level.getLevelNumber() == 3) {
            hasLevel3 = true;
            break;
          }
        }

        if (hasLevel3) {
          if (!model.isSecretPurchased()) {
            if (model.getGold() >= 150) {
              model.purchaseSecret();
              model.addGold(-150);
            }
          } else {
            if (model.getGold() >= 10) {
              model.addGold(-10);
              model.addStar(1);
            }
          }
        }
      }
      return;
    }
  }
}

void GameController::handleOrderClick(float mouseX, float mouseY) {
  const auto& orders = model.getActiveOrders();
  
  for (size_t i = 0; i < orders.size() && i < 5; ++i) {
    float elemX = GameView::PANEL_X + (GameView::PANEL_WIDTH - GameView::ELEMENT_WIDTH) / 2.0f;
    float elemY = GameView::PANEL_Y + 30.0f + i * (GameView::ELEMENT_HEIGHT + GameView::ELEMENT_PADDING);

    if (mouseX >= elemX && mouseX <= elemX + GameView::ELEMENT_WIDTH && mouseY >= elemY && mouseY <= elemY + GameView::ELEMENT_HEIGHT) {
      for (int slotIdx = 0; slotIdx < 6; ++slotIdx) {
        if (itemCells[slotIdx] != nullptr) {
          const auto& itemProps = itemCells[slotIdx]->getProperties();
          const auto& orderProps = orders[i].requiredProperties;
          const std::string& itemType = itemCells[slotIdx]->getType();
          const std::string& orderType = orders[i].itemType;

          if (itemType != orderType)
            continue;

          auto sortedItemProps = itemProps;
          auto sortedOrderProps = orderProps;
          std::sort(sortedItemProps.begin(), sortedItemProps.end());
          std::sort(sortedOrderProps.begin(), sortedOrderProps.end());

          bool allMatch = true;
          for (const auto& orderProp : sortedOrderProps) {
            if (std::find(sortedItemProps.begin(), sortedItemProps.end(), orderProp) == sortedItemProps.end()) {
              allMatch = false;
              break;
            }
          }
          
          if (allMatch) {
            int sellPrice = static_cast<int>(orderProps.size() * 10 * itemCells[slotIdx]->getSellMultiplier());
            itemCells[slotIdx] = nullptr;
            model.addGold(sellPrice);
            model.generateNewOrder(i);
            return;
          }
        }
      }
      return;
    }
  }
}

void GameController::handleTrashPotionClick(float mouseX, float mouseY) {
  const float cellSize = 80.0f;
  const float cellPadding = 10.0f;
  const float startX = GameView::TRASH_X + 20.0f;
  const float startY = GameView::TRASH_Y + 70.0f;
  for (int i = 0; i < 6; ++i) {
    float cellX = startX + i * (cellSize + cellPadding);
    float cellY = startY;
    if (mouseX >= cellX && mouseX <= cellX + cellSize &&
        mouseY >= cellY && mouseY <= cellY + cellSize) {
      
      if (itemCells[i] != nullptr) {
        itemCells[i] = nullptr;
      }
      return;
    }
  }
}

void GameController::handleBlackScreenClick(float mouseX, float mouseY, sf::RenderWindow& window) {
  sf::FloatRect restartBounds(
    sf::Vector2f(GameView::WINDOW_WIDTH / 2.0f - GameView::BTN_WIGHT - GameView::BTN_WIGHT / 2.0f - GameView::SPACING, GameView::BTN_Y),
    sf::Vector2f(GameView::BTN_WIGHT, GameView::BTN_HEIGHT));
  sf::FloatRect continueBounds(
    sf::Vector2f(GameView::WINDOW_WIDTH / 2.0f - GameView::BTN_WIGHT / 2.0f, GameView::BTN_Y),
    sf::Vector2f(GameView::BTN_WIGHT, GameView::BTN_HEIGHT));
  sf::FloatRect exitBounds(
    sf::Vector2f(GameView::WINDOW_WIDTH / 2.0f + GameView::BTN_WIGHT / 2.0f + GameView::SPACING, GameView::BTN_Y),
    sf::Vector2f(GameView::BTN_WIGHT, GameView::BTN_HEIGHT));

  //restart
  if (restartBounds.contains(sf::Vector2f(mouseX, mouseY))) {
    model = GameModel();
    showPotion = showShop = showOrders = showTrash = false;
    amuletBaseSelected = false;
    starSelected = false;
    showSecretWindow = false;
    showBlackScreen = false;
    showMenuOverlay = false;
    selectedIngredients.clear();
    selectedPotionsForAmulet.clear();
    selectedPotionsForStar.clear();
    allSlots.clear();
    itemCells.clear();
    for (int i = 0; i < 6; ++i) {
      itemCells.push_back(nullptr);
    }
    return;
  }

  //continue
  if (continueBounds.contains(sf::Vector2f(mouseX, mouseY))) {
    showBlackScreen = false;
    showMenuOverlay = false;
    return;
  }

  //exit
  if (exitBounds.contains(sf::Vector2f(mouseX, mouseY))) {
    window.close();
  }
}

void GameController::handleEvents(sf::RenderWindow& window) {
  while (const std::optional event = window.pollEvent()) {
    if (event->is<sf::Event::Closed>())
      window.close();

    if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
      if (mouseEvent->button == sf::Mouse::Button::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
        float mouseX = worldPos.x;
        float mouseY = worldPos.y;

        if (showBlackScreen || showMenuOverlay) {
          handleBlackScreenClick(mouseX, mouseY, window);
          return;
        }

        // menu
        if (mouseX >= GameView::MENU_BUTTON_X && mouseX <= GameView::MENU_BUTTON_X + GameView::MENU_BUTTON_SIZE &&
          mouseY >= GameView::MENU_BUTTON_Y && mouseY <= GameView::MENU_BUTTON_Y + GameView::MENU_BUTTON_SIZE) {
          showMenuOverlay = true;
          return;
        }

        handleShopClick(mouseX, mouseY);
        handleOrdersButtonClick(mouseX, mouseY);
        handleTrashButtonClick(mouseX, mouseY);
        handleCauldronClick(mouseX, mouseY);
        if (model.isSecretPurchased()) {
          const auto& levels = model.getUnlockedLevels();
          float totalWidth = levels.size() * GameView::LEVEL_SIZE + (levels.size() - 1) * GameView::LEVEL_PADDING;
          float startX = (GameView::WINDOW_WIDTH - totalWidth) / 2.0f;
          float secretX = startX + levels.size() * (GameView::LEVEL_SIZE + GameView::LEVEL_PADDING) + 10.0f;
          float secretY = GameView::SECRET_Y;
          if (mouseX >= secretX && mouseX <= secretX + GameView::LEVEL_SIZE && mouseY >= secretY && mouseY <= secretY + GameView::LEVEL_SIZE) {
            if (showSecretWindow) {
              showSecretWindow = false;
            } else {
              if (showPotion && !selectedIngredients.empty()) {
                for (int ingIdx : selectedIngredients) {
                  model.setIngredientQuantity(ingIdx, model.getIngredientQuantity(ingIdx) + 1);
                }
                selectedIngredients.clear();
              }
              showSecretWindow = true;
              showShop = false;
              showOrders = false;
              showTrash = false;
              showPotion = false;
              if (amuletBaseSelected) {
                model.setAmuletBases(model.getAmuletBases() + 1);
                amuletBaseSelected = false;
              }
              selectedPotionsForAmulet.clear();
            }
            return;
          }
        }
        
        if (showShop) {
          handleShopBuy(mouseX, mouseY);
        } else if (showOrders) {
          handleOrderClick(mouseX, mouseY);
        } else if (showTrash) {
          handleTrashPotionClick(mouseX, mouseY);
        } else if (showPotion) {
          handleShelfClick(mouseX, mouseY);
        }
      }
    }
  }
}

void GameController::draw(sf::RenderWindow& window) {
  sf::View gameView(sf::FloatRect({0.f, 0.f}, {GameView::WINDOW_WIDTH, GameView::WINDOW_HEIGHT}));
  window.setView(gameView);
  view.draw(window, model, showPotion, showShop, showOrders, showTrash,
    selectedIngredients, itemCells, allSlots,
    selectedPotionsForStar, selectedPotionsForAmulet,
    showSecretWindow, showBlackScreen, amuletBaseSelected, starSelected, showMenuOverlay);
}
