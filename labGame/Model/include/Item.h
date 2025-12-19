#ifndef LABGAME_ITEM_H
#define LABGAME_ITEM_H

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>


class Item {
protected:
  std::string name;
  double sellMultiplier;
  std::string imageFile;
  static constexpr int PROPERTY_VALUE = 10;

public:
  Item() : sellMultiplier(1.0) {}
  Item(const std::string& name, double sellMultiplier, std::string imageFile = "");
  virtual ~Item() = default;

  virtual void load(std::istream& is);

  const std::string& getName() const;
  const std::string& getImageFileName() const;
  const std::string& getImageFile() const;
  double getSellMultiplier() const;

  virtual std::string getType() const = 0;
  virtual int getBuyPrice() const = 0;
  virtual int getPropertyCount() const = 0;
  virtual int getSellPrice() const = 0;
  virtual const std::vector<std::string>& getProperties() const = 0;
};

#endif
