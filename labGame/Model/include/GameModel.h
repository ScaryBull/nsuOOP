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
#include <iostream>
#include <stdexcept>

#include "Item.h"
#include "Order.h"
#include "AlchemyLevel.h"
#include "Amulet.h"
#include "Ingredient.h"
#include "ItemFactories.h"
#include "Star.h"


class GameModel {
private:
  static constexpr int NUM_ORDERS = 5;
  static constexpr int INITIAL_INGREDIENT_QTY = 5;
  static constexpr int MIN_INGREDIENTS = 2;
  static constexpr int AMULET_BASE_PRICE = 50;
  static constexpr int LEVEL2_PRICE = 0;    //100
  static constexpr int LEVEL3_PRICE = 0;    //300
  static constexpr int SECRET_PRICE = 0;    //500

  std::vector<std::shared_ptr<Item>> allIngredients;
  std::map<int, int> inventory;
  std::vector<std::shared_ptr<Item>> craftedPotions;
  std::vector<Order> activeOrders;
  std::vector<AlchemyLevel> unlockedLevels;
  std::vector<AlchemyLevel> availableLevels;

  long long gold = 50;
  int amuletBases = 0;
  int starCount = 0;
  bool secretPurchased = false;
  std::mt19937 rng;

  void initializeIngredients();
  void initializeInventory();
  void initializeLevels();
  std::vector<std::string> pickUniqueIngredientProperties(int count);
  struct OrderSpec { int propertyCount; std::string itemType; };
  OrderSpec computeOrderSpec(int orderIndex);

public:
  static constexpr int NUM_INGREDIENTS = 6;
  static constexpr int TIER2_MAX_INGS = NUM_INGREDIENTS - 1;
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
  bool hasLevel(int level) const;

  int getLevelUnlockPrice(int level) const;
  int getAmuletBasePrice() const;
  int getSecretPrice() const;
  int getStarPrice() const;
  
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
