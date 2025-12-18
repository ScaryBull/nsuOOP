#ifndef LABGAME_POTION_H
#define LABGAME_POTION_H

#include <string>
#include <vector>
#include "Item.h"


class Potion : public Item {
private:
  std::vector<std::string> properties;

public:
  Potion();
  Potion(const std::string& name, std::vector<std::string> props,
    double sellMultiplier = 1.0, const std::string& imageFile = "");
  
  void load(std::istream& is) override;

  std::string getType() const override;
  int getBuyPrice() const override;
  int getPropertyCount() const override;
  int getSellPrice() const override;
  const std::vector<std::string>& getProperties() const;
  bool matchesRecipe(const std::vector<std::string>& input) const;
};

#endif
