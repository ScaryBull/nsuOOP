#ifndef LABGAME_ORDER_H
#define LABGAME_ORDER_H

#include <string>
#include <vector>

struct Order {
  std::string name;
  std::vector<std::string> requiredProperties;
  std::string itemType;

  Order(const std::string& name, const std::vector<std::string>& props, const std::string& type = "potion")
      : name(name), requiredProperties(props), itemType(type) {}
};

#endif
