#ifndef LABGAME_STAR_H
#define LABGAME_STAR_H

#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include "Item.h"
#include "ItemFactories.h"

class Star : public Item {
private:
  std::vector<std::string> properties;
  int buyPrice;

public:  Star();  Star(const std::string& name, std::vector<std::string> props,
       double sellMultiplier = 1.0, const std::string& imageFile = "star.png", int buyPrice = 10);
  
  void load(std::istream& is) override;

  std::string getType() const override;
  int getBuyPrice() const override;
  int getPropertyCount() const override;
  int getSellPrice() const override;
  const std::vector<std::string>& getProperties() const override;
  bool matchesRecipe(const std::vector<std::string>& input) const;
};

#endif
