#include "GameView.h"


GameView::GameView() {
  try {
    loadTextureOrThrow(backgroundTexture, "Wall.jpg");
    loadTextureOrThrow(shelfTexture, "shelf.png");
    loadTextureOrThrow(cauldronTexture, "cauldron.png");
    loadTextureOrThrow(potionTexture, "potion.png");
    loadTextureOrThrow(mixButtonTexture, "mix_button.png");
    loadTextureOrThrow(potionResultTexture, "potion_result.png");
    loadTextureOrThrow(amuletResultTexture, "amulet_result.png");
    loadTextureOrThrow(shopButtonTexture, "shop_button.png");
    loadTextureOrThrow(ordersButtonTexture, "orders_button.png");
    loadTextureOrThrow(trashButtonTexture, "trash.png");
    loadTextureFromFile(menuButtonTexture, "menu.png");
    loadFontOrThrow(font, "visitor1.ttf");
    font.setSmooth(false);

    std::vector<std::string> ingredientFiles = {
      "ingredient_0.png", "ingredient_1.png", "ingredient_2.png",
      "ingredient_3.png", "ingredient_4.png", "ingredient_5.png"
    };

    size_t count = std::min<size_t>(ingredientFiles.size(), static_cast<size_t>(GameModel::NUM_INGREDIENTS));
    for (size_t i = 0; i < count; ++i) {
      loadTextureOrThrow(ingredientTextures[static_cast<int>(i)], ingredientFiles[i]);
    }

    loadTextureFromFile(level2Texture, "II.png");
    loadTextureFromFile(level3Texture, "III.png");
    loadTextureFromFile(amuletTexture, "amulet.png");
    loadTextureFromFile(secretTexture, "secret.png");
    loadTextureFromFile(starTexture, "star.png");
  } catch (const std::exception& ex) {
    throw std::runtime_error(std::string("GameView initialization failed: ") + ex.what());
  }
}

bool GameView::loadTextureFromFile(sf::Texture& tex, const std::string& path) {
  return tex.loadFromFile(path);
}

void GameView::loadTextureOrThrow(sf::Texture& tex, const std::string& path) {
  if (!tex.loadFromFile(path)) {
    throw std::runtime_error(std::string("Failed to load texture: ") + path);
  }
}

void GameView::loadFontOrThrow(sf::Font& fnt, const std::string& path) {
  if (!fnt.openFromFile(path)) {
    throw std::runtime_error(std::string("Failed to load font: ") + path);
  }
}

sf::Sprite GameView::makeSprite(const sf::Texture& tex, float x, float y, float targetWidth, float targetHeight) {
  sf::Sprite s(tex);
  s.setPosition({x, y});
  if (targetWidth > 0.0f && tex.getSize().x > 0) {
    float tw = targetWidth;
    float th = (targetHeight > 0.0f ? targetHeight : targetWidth);
    s.setScale({tw / tex.getSize().x, th / tex.getSize().y});
  }
  return s;
}

sf::Text GameView::makeText(const std::string& str, unsigned int charSize, sf::Color color, float x, float y, bool center) {
  sf::Text t(font);
  t.setString(str);
  t.setCharacterSize(charSize);
  t.setFillColor(color);
  if (center) {
    t.setOrigin({t.getLocalBounds().position.x + t.getLocalBounds().size.x / 2.0f,
                 t.getLocalBounds().position.y + t.getLocalBounds().size.y / 2.0f});
  }
  
  t.setPosition({x, y});
  return t;
}

sf::RectangleShape GameView::makeRect(float width, float height, sf::Color fill, sf::Color outline, float outlineThickness, float x, float y) {
  sf::RectangleShape r(sf::Vector2f(width, height));
  r.setFillColor(fill);
  r.setOutlineColor(outline);
  r.setOutlineThickness(outlineThickness);
  r.setPosition({x, y});
  return r;
}

bool GameView::isPotionSlotSelected(int slotIdx, const std::vector<int>& selectedPotionsForAmulet, const std::vector<int>& selectedPotionsForStar) const {
  for (int selectedIdx : selectedPotionsForAmulet) {
    if (selectedIdx == slotIdx) return true;
  }
  for (int selectedIdx : selectedPotionsForStar) {
    if (selectedIdx == slotIdx) return true;
  }
  return false;
}

void GameView::drawShelves(sf::RenderWindow& window, const GameModel& model, std::vector<IngredientSlot>& allSlots, bool showPotion, bool amuletBaseSelected, bool starSelected) {
  sf::Vector2u shelfTexSize = shelfTexture.getSize();
  float shelfScaleX = static_cast<float>(SHELF_WIDTH) / shelfTexSize.x;
  float shelfScaleY = static_cast<float>(SHELF_HEIGHT) / shelfTexSize.y;

  allSlots.clear();

  for (int i = 0; i < 3; ++i) {
    float shelfY = SHELF_START_Y + i * SHELF_SPACING;
    float ingX = (SHELF_WIDTH - INGREDIENT_SIZE) / 2.0f;
    float ingXRght = (SHELF_WIDTH + INGREDIENT_SIZE) / 2.0f;
    float ingY = shelfY - (SHELF_HEIGHT + INGREDIENT_SIZE) / 2.0f;

    //left
    sf::Sprite shelfLeft(shelfTexture);
    shelfLeft.setPosition({0, shelfY});
    shelfLeft.setScale({shelfScaleX, shelfScaleY});
    window.draw(shelfLeft);

    window.draw(makeSprite(ingredientTextures[i], ingX, ingY, INGREDIENT_SIZE, INGREDIENT_SIZE));
    allSlots.push_back({ingX, ingY, i});

    sf::Text quantityLeftText = makeText(std::to_string(model.getIngredientQuantity(i)), 30, sf::Color::White,
      ingX + INGREDIENT_SIZE - 20, ingY + INGREDIENT_SIZE - 30, false);
    window.draw(quantityLeftText);

    // star
    if (i == 1 && model.isSecretPurchased() && starTexture.getSize().x > 0 && starTexture.getSize().y > 0) {
      float shelfYMid = SHELF_START_Y + SHELF_SPACING;
      float targetSize = INGREDIENT_SIZE;
      float starX = WINDOW_WIDTH - SHELF_WIDTH - 20.0f - targetSize;
      float starY = shelfYMid - (SHELF_HEIGHT + INGREDIENT_SIZE) / 2.0f;

      sf::Sprite starSprite = makeSprite(starTexture, starX, starY, targetSize, targetSize);
      if (starSelected && showPotion)
        starSprite.setColor(sf::Color::Yellow);
      window.draw(starSprite);

      sf::Text starQty = makeText(std::to_string(model.getStarCount()), 30, sf::Color::White, starX + targetSize - 20, starY + targetSize - 30, false);
      window.draw(starQty);
    }

    //right
    sf::Sprite shelfRight(shelfTexture);
    shelfRight.setPosition({WINDOW_WIDTH - SHELF_WIDTH, shelfY});
    shelfRight.setScale({shelfScaleX, shelfScaleY});
    window.draw(shelfRight);

    window.draw(makeSprite(ingredientTextures[i + 3], WINDOW_WIDTH - ingXRght, ingY, INGREDIENT_SIZE, INGREDIENT_SIZE));
    allSlots.push_back({WINDOW_WIDTH - ingXRght, ingY, i + 3});

    sf::Text quantityRightText = makeText(std::to_string(model.getIngredientQuantity(i + 3)), 30, sf::Color::White,
      WINDOW_WIDTH - ingXRght + INGREDIENT_SIZE - 20, ingY + INGREDIENT_SIZE - 30, false);
    window.draw(quantityRightText);
  }
}

void GameView::drawShelfHook(sf::RenderWindow& window, const GameModel& model, bool showPotion, bool amuletBaseSelected) {
  if (!model.hasLevel(3))
    return;

  sf::Texture hookTexture;
  if (hookTexture.loadFromFile("hook.png")) {
    float hookScale = (INGREDIENT_SIZE / hookTexture.getSize().y) / 1.5f;
    float hookWidth = hookTexture.getSize().x * hookScale;
    float hookX = SHELF_WIDTH + 20.0f + (AMULET_WIDTH - hookWidth) / 2.0f;
    float hookY = SHELF_START_Y - (SHELF_HEIGHT + INGREDIENT_SIZE) / 2.0f;
    sf::Sprite hookSprite = makeSprite(hookTexture, hookX, hookY, hookTexture.getSize().x * hookScale, hookTexture.getSize().y * hookScale);
    window.draw(hookSprite);

    sf::Texture amuletBaseTexture;
    if (amuletBaseTexture.loadFromFile("amulet.png")) {
      float amuletX = SHELF_WIDTH + 20.0f;
      float amuletY = hookY + hookTexture.getSize().y * hookScale - 30.0f;
      sf::Sprite amuletSprite = makeSprite(amuletBaseTexture, amuletX, amuletY, AMULET_WIDTH, AMULET_HEIGHT);

      if (amuletBaseSelected && showPotion)
        amuletSprite.setColor(sf::Color::Yellow);

      window.draw(amuletSprite);
      sf::Text amuletQuantity = makeText(std::to_string(model.getAmuletBases()),
        30, sf::Color::White, amuletX + 20.0f * AMULET_SCALE - 20, amuletY + 50.0f * AMULET_SCALE - 30,
        false);
      window.draw(amuletQuantity);
    }
  }
}

void GameView::drawButtons(sf::RenderWindow& window) {
  // cauldron
  window.draw(makeSprite(cauldronTexture, CAULDRON_X, CAULDRON_Y, CAULDRON_BUTTON_SIZE, CAULDRON_BUTTON_SIZE));

  // shop
  window.draw(makeSprite(shopButtonTexture, SHOP_BUTTON_X, SHOP_BUTTON_Y, SHOP_BUTTON_SIZE, SHOP_BUTTON_SIZE));

  // orders
  window.draw(makeSprite(ordersButtonTexture, ORDERS_X, ORDERS_Y, ORDERS_BUTTON_SIZE, ORDERS_BUTTON_SIZE));

  // trash
  window.draw(makeSprite(trashButtonTexture, TRASH_BUTTON_X, TRASH_BUTTON_Y, TRASH_BUTTON_SIZE, TRASH_BUTTON_SIZE));

  // menu
  if (menuButtonTexture.getSize().x > 0 && menuButtonTexture.getSize().y > 0)
    window.draw(makeSprite(menuButtonTexture, MENU_BUTTON_X, MENU_BUTTON_Y, MENU_BUTTON_SIZE, MENU_BUTTON_SIZE));
}

void GameView::drawShopIngredientCell(sf::RenderWindow& window, const GameModel& model, int index, float cellX, float cellY, float spriteX, float spriteY, float spriteSize) {
  window.draw(makeSprite(ingredientTextures[index], cellX, cellY, SHOP_CELL_SIZE, SHOP_CELL_SIZE));
  const auto& ing = model.getIngredient(index);
  if (!ing) {
    throw std::runtime_error(std::string("GameView::drawShopWindow: missing ingredient at index ") + std::to_string(index));
  }
  auto sellable = std::dynamic_pointer_cast<Sellable>(ing);
  int price = (sellable ? sellable->getBuyPrice() : 0);
  window.draw(makeText(std::to_string(price), 16, sf::Color::Yellow, cellX + 5.0f, cellY + 5.0f, false));
}

void GameView::drawShopLevelCell(sf::RenderWindow& window, const GameModel& model, int level, float cellX, float cellY, float spriteX, float spriteY, float spriteSize) {
  bool hasLevel = model.hasLevel(level);
  sf::Texture& tex = (level == 2) ? level2Texture : level3Texture;
  if (tex.getSize().x > 0 && tex.getSize().y > 0) {
    sf::Sprite levelSprite = makeSprite(tex, spriteX, spriteY, spriteSize, spriteSize);
    if (hasLevel) levelSprite.setColor(sf::Color(120, 120, 120));
    else if (level == 3 && !model.hasLevel(2)) levelSprite.setColor(sf::Color(80, 80, 80));
    window.draw(levelSprite);
  }

  if (!hasLevel) {
    int price = model.getLevelUnlockPrice(level);
    window.draw(makeText(std::to_string(price), 14, sf::Color::Yellow, cellX + 5.0f, cellY + 5.0f, false));
  }
}

void GameView::drawShopAmuletCell(sf::RenderWindow& window, const GameModel& model, float cellX, float cellY, float spriteX, float spriteY, float spriteSize) {
  bool hasLevel3 = model.hasLevel(3);
  if (hasLevel3 && amuletTexture.getSize().x > 0 && amuletTexture.getSize().y > 0) {
    window.draw(makeSprite(amuletTexture, spriteX, spriteY, spriteSize, spriteSize));
    int price = model.getAmuletBasePrice();
    window.draw(makeText(std::to_string(price), 14, sf::Color::Yellow, cellX + 5.0f, cellY + 5.0f, false));
  }
}

void GameView::drawShopSecretCell(sf::RenderWindow& window, const GameModel& model, float cellX, float cellY, float spriteX, float spriteY, float spriteSize) {
  bool hasLevel3 = model.hasLevel(3);
  if (!hasLevel3) return;

  if (model.isSecretPurchased()) {
    if (starTexture.getSize().x > 0 && starTexture.getSize().y > 0) {
      window.draw(makeSprite(starTexture, spriteX, spriteY, spriteSize, spriteSize));
    }
    window.draw(makeText(std::to_string(model.getStarPrice()), 14, sf::Color::Yellow, cellX + 5.0f, cellY + 5.0f, false));
  } else {
    if (secretTexture.getSize().x > 0 && secretTexture.getSize().y > 0) {
      window.draw(makeSprite(secretTexture, spriteX, spriteY, spriteSize, spriteSize));
    }
    window.draw(makeText(std::to_string(model.getSecretPrice()), 14, sf::Color::Yellow, cellX + 5.0f, cellY + 5.0f, false));
  }
}

void GameView::drawPotionWindow(sf::RenderWindow& window, const std::vector<int>& selectedIngredients, const std::vector<std::shared_ptr<Item>>& itemCells, const std::vector<int>& selectedPotionsForAmulet, const std::vector<int>& selectedPotionsForStar, bool amuletBaseSelected, bool starSelected) {
  window.draw(makeSprite(potionTexture, (WINDOW_WIDTH - POTION_SIZE) / 2.0f, (WINDOW_HEIGHT - POTION_SIZE) / 2.0f, POTION_SIZE, POTION_SIZE));

  if (starSelected) {
      if (starTexture.getSize().x > 0 && starTexture.getSize().y > 0) {
        float targetSize = INGREDIENT_SIZE;
        float starX = WINDOW_WIDTH / 2.0f - targetSize / 2.0f;
        float starY = WINDOW_HEIGHT / 2.0f - targetSize / 2.0f;
        window.draw(makeSprite(starTexture, starX, starY, targetSize, targetSize));
      }

    for (size_t i = 0; i < selectedPotionsForStar.size(); ++i) {
      int slotIdx = selectedPotionsForStar[i];
      if (itemCells[slotIdx] != nullptr) {
        sf::Texture potionTexture;
        if (potionTexture.loadFromFile(itemCells[slotIdx]->getImageFile())) {
          float angle = (2.0f * 3.14159f * i) / selectedPotionsForStar.size();
          float potionX = WINDOW_WIDTH / 2.0f - SCALE_SIZE / 2.0f + RADIUS * std::cos(angle);
          float potionY = WINDOW_HEIGHT / 2.0f - SCALE_SIZE / 2.0f + RADIUS * std::sin(angle);
          window.draw(makeSprite(potionTexture, potionX, potionY, SCALE_SIZE, SCALE_SIZE));
        }
      }
    }

    if (!selectedPotionsForStar.empty()) {
      float buttonX = (WINDOW_WIDTH + POTION_SIZE) / 2.0f - MIX_BUTTON_SIZE - 10;
      float buttonY = (WINDOW_HEIGHT + POTION_SIZE) / 2.0f - MIX_BUTTON_SIZE - 10;
      window.draw(makeSprite(mixButtonTexture, buttonX, buttonY, MIX_BUTTON_SIZE, MIX_BUTTON_SIZE));
    }
  } else if (amuletBaseSelected) {
    sf::Texture amuletBaseTexture;
    if (amuletBaseTexture.loadFromFile("amulet.png")) {
      float amuletScale = (INGREDIENT_SIZE / 50.0f) * 1.5f;
      float amuletX = WINDOW_WIDTH / 2.0f - (20.0f * amuletScale) / 2.0f;
      float amuletY = WINDOW_HEIGHT / 2.0f - (50.0f * amuletScale) / 2.0f;
      window.draw(makeSprite(amuletBaseTexture, amuletX, amuletY, 20.0f * amuletScale, 50.0f * amuletScale));
    }

    for (size_t i = 0; i < selectedPotionsForAmulet.size(); ++i) {
      int slotIdx = selectedPotionsForAmulet[i];
      if (itemCells[slotIdx] != nullptr) {
        sf::Texture potionTexture;
        if (potionTexture.loadFromFile(itemCells[slotIdx]->getImageFile())) {
          float angle = (2.0f * 3.14159f * i) / selectedPotionsForAmulet.size();
          float potionX = WINDOW_WIDTH / 2.0f - SCALE_SIZE / 2.0f + RADIUS * std::cos(angle);
          float potionY = WINDOW_HEIGHT / 2.0f - SCALE_SIZE / 2.0f + RADIUS * std::sin(angle);
          window.draw(makeSprite(potionTexture, potionX, potionY, SCALE_SIZE, SCALE_SIZE));
        }
      }
    }

    if (!selectedPotionsForAmulet.empty()) {
      float buttonX = (WINDOW_WIDTH + POTION_SIZE) / 2.0f - MIX_BUTTON_SIZE - 10;
      float buttonY = (WINDOW_HEIGHT + POTION_SIZE) / 2.0f - MIX_BUTTON_SIZE - 10;
      window.draw(makeSprite(mixButtonTexture, buttonX, buttonY, MIX_BUTTON_SIZE, MIX_BUTTON_SIZE));
    }
  } else {
    for (size_t i = 0; i < selectedIngredients.size(); ++i) {
      int ingIdx = selectedIngredients[i];
      int row = i / INGREDIENT_PER_POW;
      int col = i % INGREDIENT_PER_POW;
      float startX = WINDOW_WIDTH / 2.0f - SCALE_SIZE;
      float startY = (WINDOW_HEIGHT + POTION_SIZE) / 2.0f - SCALE_SIZE - 20.0f;
      float ingX = startX + col * SCALE_SIZE;
      float ingY = startY - row * SCALE_SIZE;
      window.draw(makeSprite(ingredientTextures[ingIdx], ingX, ingY, SCALE_SIZE, SCALE_SIZE));
    }

    if (selectedIngredients.size() >= 2) {
      float buttonX = (WINDOW_WIDTH + POTION_SIZE) / 2.0f - MIX_BUTTON_SIZE - 10;
      float buttonY = (WINDOW_HEIGHT + POTION_SIZE) / 2.0f - MIX_BUTTON_SIZE - 10;
      window.draw(makeSprite(mixButtonTexture, buttonX, buttonY, MIX_BUTTON_SIZE, MIX_BUTTON_SIZE));
    }
  }
}

void GameView::drawPotionSlots(sf::RenderWindow& window, const std::vector<std::shared_ptr<Item>>& itemCells, const std::vector<int>& selectedPotionsForAmulet, const std::vector<int>& selectedPotionsForStar) {
  for (int i = 0; i < GameModel::NUM_INGREDIENTS; ++i) {
    float cellX = POTION_CELL_PADDING + i * (POTION_CELL_SIZE + POTION_CELL_PADDING);
    float cellY = WINDOW_HEIGHT - POTION_CELL_SIZE - POTION_CELL_PADDING;

    bool isSelected = isPotionSlotSelected(i, selectedPotionsForAmulet, selectedPotionsForStar);

    sf::RectangleShape cell = makeRect(POTION_CELL_SIZE, POTION_CELL_SIZE, sf::Color(50,50,50,200), isSelected ? sf::Color::Yellow : sf::Color::White, 2.0f, cellX, cellY);
    window.draw(cell);

    if (itemCells[i] != nullptr) {
      sf::Texture itemTexture;
      if (itemCells[i]->getType() == "amulet") {
        loadTextureFromFile(itemTexture, "amulet_result.png");
      } else {
        itemTexture = potionResultTexture;
      }
      
      window.draw(makeSprite(itemTexture, cellX, cellY, POTION_CELL_SIZE, POTION_CELL_SIZE));
      const auto& properties = itemCells[i]->getProperties();
      float startY = cellY - 10.0f - (properties.size() - 1) * 18.0f;
      for (size_t j = 0; j < properties.size(); ++j) {
        sf::Text propertyText = makeText(properties[j], 14, sf::Color::Yellow, cellX + POTION_CELL_SIZE / 2.0f,
          startY + j * 18.0f, true);
        window.draw(propertyText);
      }
    }
  }
}

void GameView::drawShopWindow(sf::RenderWindow& window, const GameModel& model) {
  const auto& levels = model.getUnlockedLevels();
  sf::RectangleShape shopBg = makeRect(SHOP_WIDTH, SHOP_HEIGHT, sf::Color(139,69,19), sf::Color::White, 3.0f, SHOP_X, SHOP_Y);
  window.draw(shopBg);

  for (int i = 0; i < 10; ++i) {
    int col = i % COLS;
    int row = i / COLS;
    float cellX = SHOP_X + 15 + col * (SHOP_CELL_SIZE + 8);
    float cellY = SHOP_Y + 15 + row * (SHOP_CELL_SIZE + 8);
    float innerPad = GameView::SHOP_CELL_INNER_PAD;
    float spriteX = cellX + innerPad;
    float spriteY = cellY + innerPad;
    float spriteSize = static_cast<float>(SHOP_CELL_SIZE) - 2.0f * innerPad;

    sf::RectangleShape cell = makeRect(SHOP_CELL_SIZE, SHOP_CELL_SIZE, sf::Color(100,100,100), sf::Color::Transparent, 0.0f, cellX, cellY);
    window.draw(cell);

    if (i < GameModel::NUM_INGREDIENTS) {
      drawShopIngredientCell(window, model, i, cellX, cellY, spriteX, spriteY, spriteSize);
    } else if (i == GameView::SHOP_IDX_LEVEL2) {
      drawShopLevelCell(window, model, 2, cellX, cellY, spriteX, spriteY, spriteSize);
    } else if (i == GameView::SHOP_IDX_LEVEL3) {
      drawShopLevelCell(window, model, 3, cellX, cellY, spriteX, spriteY, spriteSize);
    } else if (i == GameView::SHOP_IDX_AMULET) {
      drawShopAmuletCell(window, model, cellX, cellY, spriteX, spriteY, spriteSize);
    } else if (i == GameView::SHOP_IDX_SECRET) {
      drawShopSecretCell(window, model, cellX, cellY, spriteX, spriteY, spriteSize);
    }
  }
  
  sf::Text shopInstruction = makeText("Click to buy", 18, sf::Color::White, SHOP_X + SHOP_WIDTH / 2.0f, SHOP_Y + SHOP_HEIGHT - 35.0f, true);
  window.draw(shopInstruction);
}

void GameView::drawTrashWindow(sf::RenderWindow& window, const std::vector<std::shared_ptr<Item>>& itemCells) {
  sf::RectangleShape trashPanel = makeRect(TRASH_WIDTH, TRASH_HEIGHT, sf::Color(100,50,50), sf::Color::White, 3.0f, TRASH_X, TRASH_Y);
  window.draw(trashPanel);

  const float cellSize = 80.0f;
  const float cellPadding = 10.0f;
  const float startX = TRASH_X + 20.0f;
  const float startY = TRASH_Y + 70.0f;
  
  for (int i = 0; i < GameModel::NUM_INGREDIENTS; ++i) {
    float cellX = startX + i * (cellSize + cellPadding);
    float cellY = startY;

    sf::RectangleShape cell = makeRect(cellSize, cellSize, sf::Color(70,70,70), sf::Color::White, 2.0f, cellX, cellY);
    window.draw(cell);

    if (itemCells[i] != nullptr) {
      window.draw(makeSprite(potionResultTexture, cellX, cellY, cellSize, cellSize));

      const auto& properties = itemCells[i]->getProperties();
      float textStartY = cellY - 10.0f - (properties.size() - 1) * 15.0f;
      
      for (size_t j = 0; j < properties.size(); ++j) {
        sf::Text propertyText = makeText(properties[j], 12, sf::Color::Yellow, cellX + cellSize / 2.0f, textStartY + j * 15.0f, true);
        window.draw(propertyText);
      }
    }
  }
  
  sf::Text instructionText = makeText("Click on potion to delete", 18, sf::Color::White, TRASH_X + TRASH_WIDTH / 2.0f, TRASH_Y + TRASH_HEIGHT - 25.0f, true);
  window.draw(instructionText);
}

void GameView::drawOrdersWindow(sf::RenderWindow& window, const GameModel& model) {
  sf::RectangleShape orderPanel = makeRect(PANEL_WIDTH, PANEL_HEIGHT, sf::Color(245,222,179), sf::Color::White, 3.0f, PANEL_X, PANEL_Y);
  window.draw(orderPanel);
  const auto& orders = model.getActiveOrders();
  for (size_t i = 0; i < orders.size() && i < 5; ++i) {
    float elemX = PANEL_X + (PANEL_WIDTH - ELEMENT_WIDTH) / 2.0f;
    float elemY = PANEL_Y + 30.0f + i * (ELEMENT_HEIGHT + ELEMENT_PADDING);
    
    sf::RectangleShape orderItem = makeRect(ELEMENT_WIDTH, ELEMENT_HEIGHT, sf::Color(210,180,140), sf::Color::Transparent, 0.0f, elemX, elemY);
    window.draw(orderItem);

    sf::Texture& iconTexture = (orders[i].itemType == "amulet") ? amuletResultTexture : potionResultTexture;
    float iconSize = ELEMENT_HEIGHT - 8.0f;
    window.draw(makeSprite(iconTexture, elemX + 4.0f, elemY + 4.0f, iconSize, iconSize));

    std::string propertiesText;
    for (size_t j = 0; j < orders[i].requiredProperties.size(); ++j) {
      propertiesText += orders[i].requiredProperties[j];
      if (j < orders[i].requiredProperties.size() - 1) propertiesText += ", ";
    }

    window.draw(makeText(propertiesText, 16, sf::Color::Black, elemX + iconSize + 12.0f, elemY + 10.0f, false));
  }
}

void GameView::drawTopBar(sf::RenderWindow& window, const GameModel& model) {
  const auto& levels = model.getUnlockedLevels();
  
  float totalWidth = levels.size() * LEVEL_SIZE + (levels.size() - 1) * LEVEL_PADDING;
  float startX = (WINDOW_WIDTH - totalWidth) / 2.0f;
  for (size_t i = 0; i < levels.size(); ++i) {
    sf::Texture levelTexture;
    if (levelTexture.loadFromFile(levels[i].getImageName())) {
      float levelX = startX + i * (LEVEL_SIZE + LEVEL_PADDING);
      window.draw(makeSprite(levelTexture, levelX, LEVEL_Y, LEVEL_SIZE, LEVEL_SIZE));
    }
  }

  if (model.isSecretPurchased() && secretTexture.getSize().x > 0 && secretTexture.getSize().y > 0) {
    float secretX = startX + levels.size() * (LEVEL_SIZE + LEVEL_PADDING) + 10.0f;
    window.draw(makeSprite(secretTexture, secretX, SECRET_Y, LEVEL_SIZE, LEVEL_SIZE));
  }

  window.draw(makeText("Gold: " + std::to_string(model.getGold()), 30, sf::Color::Yellow, WINDOW_WIDTH - 200.0f, 5.0f, false));
}

void GameView::drawSecretModal(sf::RenderWindow& window) {
  const float panelW = 520.0f;
  const float panelH = 180.0f;
  const float panelX = (WINDOW_WIDTH - panelW) / 2.0f;
  const float panelY = (WINDOW_HEIGHT - panelH) / 2.0f;

  sf::RectangleShape panel = makeRect(panelW, panelH, sf::Color(30,30,30,230), sf::Color::White, 2.0f, panelX, panelY);
  window.draw(panel);

  sf::Text secretText = makeText("This scroll gives you the remaining knowledge you \nneeded to achieve your goal: the Philosopher's Stone.\nAll you need to do is retrieve the star and imbue it \nwith all its properties using a potion.", 16, sf::Color::White, panelX + 20.0f, panelY + 20.0f, false);
  window.draw(secretText);
}

void GameView::drawBlackScreenOverlay(sf::RenderWindow& window, bool showBlackScreen) {
  sf::RectangleShape black = makeRect(WINDOW_WIDTH, WINDOW_HEIGHT, sf::Color::Black, sf::Color::Transparent, 0.0f, 0.0f, 0.0f);
  window.draw(black);

  if (showBlackScreen) {
    sf::Text msg = makeText("Hooray, you got the philosopher's stone!", 28, sf::Color::White,
      WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f - 100.0f, true);
    window.draw(msg);
  }


  sf::RectangleShape restartBtn = makeRect(BTN_WIGHT, BTN_HEIGHT, sf::Color(60,120,60),
    sf::Color::Transparent, 0.0f, WINDOW_WIDTH / 2.0f - BTN_WIGHT - BTN_WIGHT / 2.0f - SPACING, BTN_Y);
  window.draw(restartBtn);

  window.draw(makeText("Restart", 22, sf::Color::White,
    restartBtn.getPosition().x + BTN_WIGHT / 2.0f, restartBtn.getPosition().y + BTN_HEIGHT / 2.0f - 4.0f, true));

  sf::RectangleShape continueBtn = makeRect(BTN_WIGHT, BTN_HEIGHT, sf::Color(60,60,120),
    sf::Color::Transparent, 0.0f, WINDOW_WIDTH / 2.0f - BTN_WIGHT / 2.0f, BTN_Y);
  window.draw(continueBtn);

  window.draw(makeText("Continue", 22, sf::Color::White,
    continueBtn.getPosition().x + BTN_WIGHT / 2.0f, continueBtn.getPosition().y + BTN_HEIGHT / 2.0f - 4.0f, true));

  sf::RectangleShape exitBtn = makeRect(BTN_WIGHT, BTN_HEIGHT, sf::Color(120,60,60),
    sf::Color::Transparent, 0.0f, WINDOW_WIDTH / 2.0f + BTN_WIGHT / 2.0f + SPACING, BTN_Y);
  window.draw(exitBtn);

  window.draw(makeText("Exit", 22, sf::Color::White,
    exitBtn.getPosition().x + BTN_WIGHT / 2.0f, exitBtn.getPosition().y + BTN_HEIGHT / 2.0f - 4.0f, true));
}

void GameView::draw(sf::RenderWindow& window, const GameModel& model, bool showPotion, bool showShop, bool showOrders, bool showTrash,
  const std::vector<int>& selectedIngredients, const std::vector<std::shared_ptr<Item>>& itemCells, std::vector<IngredientSlot>& allSlots,
  const std::vector<int>& selectedPotionsForStar, const std::vector<int>& selectedPotionsForAmulet,
  bool showSecretWindow, bool showBlackScreen, bool amuletBaseSelected, bool starSelected, bool showMenuOverlay) {

  window.clear(sf::Color::Black);
  window.draw(makeSprite(backgroundTexture, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));

  drawShelves(window, model, allSlots, showPotion, amuletBaseSelected, starSelected);
  drawShelfHook(window, model, showPotion, amuletBaseSelected);
  drawButtons(window);

  using Handler = std::pair<std::function<bool()>, std::function<void()>>;
  std::vector<Handler> handlers;

  handlers.push_back({[&]() { return showPotion; }, [&]() { drawPotionWindow(window, selectedIngredients, itemCells, selectedPotionsForAmulet, selectedPotionsForStar, amuletBaseSelected, starSelected); }});
  handlers.push_back({[&]() { return true; }, [&]() { drawPotionSlots(window, itemCells, selectedPotionsForAmulet, selectedPotionsForStar); }});
  handlers.push_back({[&]() { return showShop; }, [&]() { drawShopWindow(window, model); }});
  handlers.push_back({[&]() { return showTrash; }, [&]() { drawTrashWindow(window, itemCells); }});
  handlers.push_back({[&]() { return true; }, [&]() { drawTopBar(window, model); }});
  handlers.push_back({[&]() { return showOrders && !showPotion && !showShop && !showTrash; }, [&]() { drawOrdersWindow(window, model); }});
  handlers.push_back({[&]() { return showSecretWindow; }, [&]() { drawSecretModal(window); }});
  handlers.push_back({[&]() { return showBlackScreen || showMenuOverlay; }, [&]() { drawBlackScreenOverlay(window, showBlackScreen); }});

  for (const auto &h : handlers) {
    if (h.first()) h.second();
  }
}
