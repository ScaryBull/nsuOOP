#ifndef LABGAME_SELLABLE_H
#define LABGAME_SELLABLE_H

#include <memory>

class Sellable {
public:
  virtual ~Sellable() = default;
  virtual int getBuyPrice() const = 0;
  virtual int getSellPrice() const = 0;
};

#endif
