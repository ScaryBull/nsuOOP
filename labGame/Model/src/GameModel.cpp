#include "../include/GameModel.h"


void GameModel::initializeIngredients() {
    struct InitEntry { const char* name; const char* prop; int price; double mul; const char* img; } entries[] = {
      {"Mandragora", "Healing", INITIAL_INGREDIENT_QTY, 1.0, "ingredient_0.png"},
      {"Ginger root", "Energy", INITIAL_INGREDIENT_QTY, 1.0, "ingredient_1.png"},
      {"Manticore ashes", "Strenght", INITIAL_INGREDIENT_QTY, 1.0, "ingredient_2.png"},
      {"Phoenix feather", "Revival", INITIAL_INGREDIENT_QTY, 1.0, "ingredient_3.png"},
      {"Hydra's fang", "Poison", INITIAL_INGREDIENT_QTY, 1.0, "ingredient_4.png"},
      {"Moon mushroom", "Invisibility", INITIAL_INGREDIENT_QTY, 1.0, "ingredient_5.png"}
    };

    for (const auto &e : entries) {
      std::ostringstream oss;
      oss << std::quoted(std::string(e.name)) << ' ' << e.mul << ' ' << std::quoted(std::string(e.img)) << ' ' << std::quoted(std::string(e.prop)) << ' ' << e.price;
      std::istringstream iss(oss.str());
      std::shared_ptr<Item> raw;
      try {
        raw = ItemFactory::Instance().CreateObject(std::string("Ingredient"));
      } catch (const std::exception& ex) {
        std::cerr << "GameModel::initializeIngredients: factory threw std::exception: " << ex.what() << '\n';
        raw = std::make_shared<Ingredient>();
      } catch (...) {
        std::cerr << "GameModel::initializeIngredients: unknown exception from factory" << '\n';
        raw = std::make_shared<Ingredient>();
      }
      raw->load(iss);
      allIngredients.emplace_back(raw);
    }
  }

void GameModel::initializeInventory() {
    for (int i = 0; i < NUM_INGREDIENTS; ++i) {
      inventory[i] = INITIAL_INGREDIENT_QTY;
    }
  }

void GameModel::initializeLevels() {
    availableLevels.clear();
    availableLevels.emplace_back(1, MIN_INGREDIENTS, "I.png", 0);
    availableLevels.emplace_back(2, 5, "II.png", LEVEL2_PRICE);
    availableLevels.emplace_back(3, 5, "III.png", LEVEL3_PRICE);

    auto it = std::find_if(availableLevels.begin(), availableLevels.end(), [](const AlchemyLevel& l){ return l.getLevelNumber() == 1; });
    if (it != availableLevels.end()) {
      unlockedLevels.push_back(*it);
    } else {
      throw std::runtime_error("GameModel::initializeLevels: required level 1 missing in availableLevels");
    }
  }

GameModel::GameModel() : rng(std::chrono::steady_clock::now().time_since_epoch().count()) {
  initializeIngredients();
  initializeInventory();
  initializeLevels();
  regenerateOrdersForLevel();
}

int GameModel::getIngredientQuantity(int index) const {
  auto it = inventory.find(index);
  return (it != inventory.end()) ? it->second : 0;
}

void GameModel::setIngredientQuantity(int index, int quantity) {
  inventory[index] = std::max(0, quantity);
}

bool GameModel::decrementIngredient(int index) {
  if (inventory.count(index) && inventory[index] > 0) {
    inventory[index]--;
    return true;
  }
  return false;
}

const std::shared_ptr<Item>& GameModel::getIngredient(int index) const {
  return allIngredients.at(index);
}

void GameModel::addPotion(const std::shared_ptr<Item>& potion) {
  craftedPotions.push_back(potion);
}

const std::vector<std::shared_ptr<Item>>& GameModel::getCraftedPotions() const {
  return craftedPotions;
}

const std::vector<Order>& GameModel::getActiveOrders() const {
  return activeOrders;
}

void GameModel::removeOrder(size_t index) {
  if (index < activeOrders.size()) {
    activeOrders.erase(activeOrders.begin() + index);
  }
}

void GameModel::generateNewOrder(int orderIndex) {
  auto spec = computeOrderSpec(orderIndex);
  auto props = pickUniqueIngredientProperties(spec.propertyCount);

  std::string orderName = "Order " + std::to_string(orderIndex + 1);
  if (orderIndex >= 0 && orderIndex < static_cast<int>(activeOrders.size())) {
    activeOrders[orderIndex] = Order(orderName, props, spec.itemType);
  } else {
    activeOrders.emplace_back(orderName, props, spec.itemType);
  }
}

void GameModel::regenerateOrdersForLevel() {
  activeOrders.clear();
  for (int i = 0; i < NUM_ORDERS; ++i) {
    generateNewOrder(i);
  }
}

std::vector<std::string> GameModel::pickUniqueIngredientProperties(int count) {
  std::uniform_int_distribution<int> dist(0, NUM_INGREDIENTS - 1);
  std::vector<int> usedIndices;
  std::vector<std::string> props;
  props.reserve(count);

  for (int i = 0; i < count; ++i) {
    int idx = dist(rng);
    while (std::find(usedIndices.begin(), usedIndices.end(), idx) != usedIndices.end()) {
      idx = dist(rng);
    }
    usedIndices.push_back(idx);
    props.push_back(allIngredients[idx]->getProperties().front());
  }
  return props;
}

void GameModel::addGold(int amount) {
  gold += amount;
}

long long GameModel::getGold() const {
  return gold;
}

const std::vector<AlchemyLevel>& GameModel::getUnlockedLevels() const {
  return unlockedLevels;
}

void GameModel::unlockLevel(const AlchemyLevel& level) {
  for (const auto& unlockedLevel : unlockedLevels) {
    if (unlockedLevel.getLevelNumber() == level.getLevelNumber())
      return;
  }
  unlockedLevels.push_back(level);
}

int GameModel::getMaxIngredients() const {
  int maxIngs = MIN_INGREDIENTS;
  for (const auto& level : unlockedLevels) {
    if (level.getMaxIngredients() > maxIngs)
      maxIngs = level.getMaxIngredients();
  }
  if (secretPurchased)
    maxIngs = NUM_INGREDIENTS;
  return maxIngs;
}

bool GameModel::hasLevel(int level) const {
  for (const auto& l : unlockedLevels) {
    if (l.getLevelNumber() == level) return true;
  }
  return false;
}

int GameModel::getAmuletBases() const {
  return amuletBases;
}

void GameModel::setAmuletBases(int quantity) {
  amuletBases = quantity;
}

void GameModel::decrementAmuletBase() {
  if (amuletBases > 0) {
    amuletBases--;
  }
}

bool GameModel::isSecretPurchased() const {
  return secretPurchased;
}

void GameModel::purchaseSecret() {
  secretPurchased = true;
}

int GameModel::getStarCount() const {
  return starCount;
}

void GameModel::addStar(int amount) {
  starCount = std::max(0, starCount + amount);
}

bool GameModel::decrementStar() {
  if (starCount > 0) {
    --starCount;
    return true;
  }
  return false;
}

int GameModel::getLevelUnlockPrice(int level) const {
  for (const auto &l : availableLevels) {
    if (l.getLevelNumber() == level) return l.getPrice();
  }
  return 0;
}

int GameModel::getAmuletBasePrice() const {
  return AMULET_BASE_PRICE;
}

int GameModel::getSecretPrice() const {
  return SECRET_PRICE;
}

int GameModel::getStarPrice() const {
  Star s;
  return s.getBuyPrice();
}

GameModel::OrderSpec GameModel::computeOrderSpec(int orderIndex) {
  int maxIngs = getMaxIngredients();
  bool hasLevel3 = hasLevel(3);

  enum class OrderMode { BaseMin, LimitedTier, Level3Unlocked };
  OrderMode mode = OrderMode::BaseMin;
  if (maxIngs ==MIN_INGREDIENTS)
    mode = OrderMode::BaseMin;
  else if (maxIngs == TIER2_MAX_INGS && !hasLevel3) 
    mode = OrderMode::LimitedTier;
  else if (hasLevel3) 
    mode = OrderMode::Level3Unlocked;

  int propertyCount = MIN_INGREDIENTS;
  std::string itemType = "potion";

  switch (mode) {
    case OrderMode::BaseMin: {
      propertyCount = MIN_INGREDIENTS;
      itemType = "potion";
      break;
    }
    case OrderMode::LimitedTier: {
      std::vector<int> propertyCounts;
      propertyCounts.reserve(TIER2_MAX_INGS - MIN_INGREDIENTS + 2);
      propertyCounts.push_back(MIN_INGREDIENTS);
      propertyCounts.push_back(MIN_INGREDIENTS);
      for (int v = MIN_INGREDIENTS + 1; v <= TIER2_MAX_INGS; ++v) propertyCounts.push_back(v);
      if (orderIndex >= 0 && orderIndex < static_cast<int>(propertyCounts.size())) {
        propertyCount = propertyCounts[orderIndex];
      } else {
        propertyCount = MIN_INGREDIENTS;
      }
      itemType = "potion";
      break;
    }
    case OrderMode::Level3Unlocked: {
      if (orderIndex == 0) {
        std::uniform_int_distribution<int> propCountDist(MIN_INGREDIENTS, TIER2_MAX_INGS);
        propertyCount = propCountDist(rng);
        itemType = "amulet";
      } else {
        std::vector<int> propertyCounts;
        propertyCounts.reserve(TIER2_MAX_INGS - MIN_INGREDIENTS + 1);
        propertyCounts.push_back(MIN_INGREDIENTS);
        for (int v = MIN_INGREDIENTS + 1; v <= TIER2_MAX_INGS; ++v) propertyCounts.push_back(v);
        int mappedIndex = orderIndex - 1;
        if (mappedIndex >= 0 && mappedIndex < static_cast<int>(propertyCounts.size())) {
          propertyCount = propertyCounts[mappedIndex];
        } else {
          propertyCount = MIN_INGREDIENTS;
        }
        itemType = "potion";
      }
      break;
    }
  }

  return {propertyCount, itemType};
}