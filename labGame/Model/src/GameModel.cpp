#include "../include/GameModel.h"


void GameModel::initializeIngredients() {
    struct InitEntry { const char* name; const char* prop; int price; double mul; const char* img; } entries[] = {
      {"Mandragora", "Healing", 10, 1.0, "ingredient_0.png"},
      {"Ginger root", "Energy", 15, 1.0, "ingredient_1.png"},
      {"Manticore ashes", "Strenght", 20, 1.0, "ingredient_2.png"},
      {"Phoenix feather", "Revival", 12, 1.0, "ingredient_3.png"},
      {"Hydra's fang", "Poison", 25, 1.0, "ingredient_4.png"},
      {"Moon mushroom", "Invisibility", 30, 1.0, "ingredient_5.png"}
    };

    for (const auto &e : entries) {
      std::ostringstream oss;
      oss << std::quoted(std::string(e.name)) << ' ' << e.mul << ' ' << std::quoted(std::string(e.img)) << ' ' << std::quoted(std::string(e.prop)) << ' ' << e.price;
      std::istringstream iss(oss.str());
      std::shared_ptr<Item> raw;
      try {
        raw = ItemFactory::Instance().CreateObject(std::string("Ingredient"));
      } catch (...) {
        raw = std::make_shared<Ingredient>();
      }
      raw->load(iss);
      allIngredients.emplace_back(raw);
    }
  }

void GameModel::initializeInventory() {
    for (int i = 0; i < 6; ++i) {
      inventory[i] = 5;
    }
  }

void GameModel::initializeLevels() {
    unlockedLevels.emplace_back(1, 2, "I.png");
  }

void GameModel::generateOrders() {
  activeOrders.clear();
  std::uniform_int_distribution dist(0, 5);
  for (int i = 0; i < 5; ++i) {
    int idx1 = dist(rng);
    int idx2 = dist(rng);
    while (idx2 == idx1) {
      idx2 = dist(rng);
    }
    
    std::vector<std::string> props;
    props.push_back(allIngredients[idx1]->getProperties().front());
    props.push_back(allIngredients[idx2]->getProperties().front());
    std::string orderName = "Order " + std::to_string(i + 1);
    activeOrders.emplace_back(orderName, props);
  }
}

GameModel::GameModel() : rng(std::chrono::steady_clock::now().time_since_epoch().count()) {
  initializeIngredients();
  initializeInventory();
  initializeLevels();
  generateOrders();
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
  std::uniform_int_distribution dist(0, 5);
  int maxIngs = getMaxIngredients();
  bool hasLevel3 = false;
  for (const auto& level : unlockedLevels) {
    if (level.getLevelNumber() == 3) {
      hasLevel3 = true;
      break;
    }
  }

  int propertyCount = 2;
  std::string itemType = "potion";
  if (maxIngs == 2) {
    propertyCount = 2;
    itemType = "potion";
  } else if (maxIngs == 5 && !hasLevel3) {
    std::vector propertyCounts = {2, 2, 3, 4, 5};
    if (orderIndex >= 0 && orderIndex < 5) {
      propertyCount = propertyCounts[orderIndex];
    }
    itemType = "potion";
  } else if (hasLevel3) {
    if (orderIndex == 0) {
      std::uniform_int_distribution propCountDist(2, 5);
      propertyCount = propCountDist(rng);
      itemType = "amulet";
    } else {
      std::vector propertyCounts = {2, 3, 4, 5};
      if (orderIndex >= 1 && orderIndex < 5) {
        propertyCount = propertyCounts[orderIndex - 1];
      }
      itemType = "potion";
    }
  }
  
  std::vector<std::string> props;
  std::vector<int> usedIndices;
  
  for (int i = 0; i < propertyCount; ++i) {
    int idx = dist(rng);
    while (std::find(usedIndices.begin(), usedIndices.end(), idx) != usedIndices.end()) {
      idx = dist(rng);
    }
    usedIndices.push_back(idx);
    props.push_back(allIngredients[idx]->getProperties().front());
  }
  
  std::string orderName = "Order " + std::to_string(orderIndex + 1);
  if (orderIndex >= 0 && orderIndex < static_cast<int>(activeOrders.size())) {
    activeOrders[orderIndex] = Order(orderName, props, itemType);
  } else {
    activeOrders.emplace_back(orderName, props, itemType);
  }
}

void GameModel::regenerateOrdersForLevel() {
  activeOrders.clear();
  
  int maxIngs = getMaxIngredients();
  bool hasLevel3 = false;
  for (const auto& level : unlockedLevels) {
    if (level.getLevelNumber() == 3) {
      hasLevel3 = true;
      break;
    }
  }
  
  std::uniform_int_distribution dist(0, 5);
  if (maxIngs == 2) {
    for (int i = 0; i < 5; ++i) {
      std::vector<int> usedIndices;
      std::vector<std::string> props;
      
      for (int j = 0; j < 2; ++j) {
        int idx = dist(rng);
        while (std::find(usedIndices.begin(), usedIndices.end(), idx) != usedIndices.end()) {
          idx = dist(rng);
        }
        usedIndices.push_back(idx);
        props.push_back(allIngredients[idx]->getProperties().front());
      }
      
      std::string orderName = "Order " + std::to_string(i + 1);
      activeOrders.emplace_back(orderName, props, "potion");
    }
  } else if (maxIngs == 5 && !hasLevel3) {
    std::vector propertyCounts = {2, 2, 3, 4, 5};
    
    for (size_t i = 0; i < propertyCounts.size(); ++i) {
      int propertyCount = propertyCounts[i];
      std::vector<int> usedIndices;
      std::vector<std::string> props;
      
      for (int j = 0; j < propertyCount; ++j) {
        int idx = dist(rng);
        while (std::find(usedIndices.begin(), usedIndices.end(), idx) != usedIndices.end()) {
          idx = dist(rng);
        }
        usedIndices.push_back(idx);
        props.push_back(allIngredients[idx]->getProperties().front());
      }
      
      std::string orderName = "Order " + std::to_string(i + 1);
      activeOrders.emplace_back(orderName, props, "potion");
    }
  } else if (hasLevel3) {
    std::uniform_int_distribution propCountDist(2, 5);
    int propertyCount = propCountDist(rng);
    std::vector<int> usedIndices;
    std::vector<std::string> props;
    
    for (int j = 0; j < propertyCount; ++j) {
      int idx = dist(rng);
      while (std::find(usedIndices.begin(), usedIndices.end(), idx) != usedIndices.end()) {
        idx = dist(rng);
      }
      usedIndices.push_back(idx);
      props.push_back(allIngredients[idx]->getProperties().front());
    }
    
    activeOrders.emplace_back("Order 1", props, "amulet");
    std::vector propertyCounts = {2, 3, 4, 5};
    for (size_t i = 0; i < propertyCounts.size(); ++i) {
      int propertyCount = propertyCounts[i];
      std::vector<int> usedIndices;
      std::vector<std::string> props;
      
      for (int j = 0; j < propertyCount; ++j) {
        int idx = dist(rng);
        while (std::find(usedIndices.begin(), usedIndices.end(), idx) != usedIndices.end()) {
          idx = dist(rng);
        }
        usedIndices.push_back(idx);
        props.push_back(allIngredients[idx]->getProperties().front());
      }
      
      std::string orderName = "Order " + std::to_string(i + 2);
      activeOrders.emplace_back(orderName, props, "potion");
    }
  }
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
  int maxIngs = 2;
  for (const auto& level : unlockedLevels) {
    if (level.getMaxIngredients() > maxIngs)
      maxIngs = level.getMaxIngredients();
  }
  if (secretPurchased)
    maxIngs = 6;
  return maxIngs;
}

bool GameModel::hasLevel3() const {
  for (const auto& level : unlockedLevels) {
    if (level.getLevelNumber() == 3)
      return true;
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