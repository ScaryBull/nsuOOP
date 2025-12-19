#ifndef LABGAME_INGREDIENT_H
#define LABGAME_INGREDIENT_H

#include <string>
#include <iomanip>
#include "Item.h"
#include "ItemFactories.h"


class Ingredient : public Item {
private:
  std::string property;
  int buyPrice;
  mutable std::vector<std::string> propertiesCache;

public:
  Ingredient();
  Ingredient(const std::string& name, const std::string& property, int buyPrice,
    double sellMultiplier = 1.0, const std::string& imageFile = "");
  
  void load(std::istream& is) override;

  std::string getType() const override;
  int getBuyPrice() const override;
  int getPropertyCount() const override;
  int getSellPrice() const override;
  const std::vector<std::string>& getProperties() const override;
  const std::string& getProperty() const;
};

#endif
