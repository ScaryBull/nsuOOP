#ifndef LABGAME_GAMEMODEL_H
#define LABGAME_GAMEMODEL_H

#include <algorithm>
#include <map>
#include <vector>
#include <memory>
#include <random>
#include <chrono>
#include <sstream>
#include <iomanip>
#include "Item.h"
#include "Order.h"
#include "AlchemyLevel.h"
#include "Amulet.h"
#include "Ingredient.h"
#include "ItemFactories.h"

class GameModel {
private:
  std::vector<std::shared_ptr<Item>> allIngredients;
  std::map<int, int> inventory;
  std::vector<std::shared_ptr<Item>> craftedPotions;
  std::vector<Order> activeOrders;
  std::vector<AlchemyLevel> unlockedLevels;

  long long gold = 100;
  int amuletBases = 0;
  int starCount = 0;
  bool secretPurchased = false;
  std::mt19937 rng;

  void initializeIngredients();
  void initializeInventory();
  void initializeLevels();
  void generateOrders();

public:
  GameModel();

  int getIngredientQuantity(int index) const;
  void setIngredientQuantity(int index, int quantity) ;
  bool decrementIngredient(int index);
  const std::shared_ptr<Item>& getIngredient(int index) const;

  void addPotion(const std::shared_ptr<Item>& potion);
  const std::vector<std::shared_ptr<Item>>& getCraftedPotions() const;
  
  const std::vector<Order>& getActiveOrders() const;
  void removeOrder(size_t index);
  void generateNewOrder(int orderIndex);
  void regenerateOrdersForLevel();
  void addGold(int amount);
  long long getGold() const;
  
  const std::vector<AlchemyLevel>& getUnlockedLevels() const;
  void unlockLevel(const AlchemyLevel& level);
  int getMaxIngredients() const;
  bool hasLevel3() const;
  
  int getAmuletBases() const;
  void setAmuletBases(int quantity);
  void decrementAmuletBase();
  int getStarCount() const;
  void addStar(int amount);
  bool decrementStar();
  bool isSecretPurchased() const;
  void purchaseSecret();
};

#endif
