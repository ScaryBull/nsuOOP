#ifndef LABGAME_AMULET_H
#define LABGAME_AMULET_H

#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include "Item.h"
#include "ItemFactories.h"


class Amulet : public Item {
private:
  std::vector<std::string> properties;

public:
  Amulet();
  Amulet(const std::string& name, std::vector<std::string> props,
    double sellMultiplier = 1.0, const std::string& imageFile = "result_amulet.png");
  
  void load(std::istream& is) override;

  std::string getType() const override;
  int getBuyPrice() const override;
  int getPropertyCount() const override;
  int getSellPrice() const override;
  const std::vector<std::string>& getProperties() const;
  bool matchesRecipe(const std::vector<std::string>& input) const;
};

#endif
