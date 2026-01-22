#include "../include/Star.h"


Star::Star() : Item("", 1.0), properties(), buyPrice(10) {}

Star::Star(const std::string& name, std::vector<std::string> props,
  double sellMultiplier, const std::string& imageFile, int buyPrice)
  : Item(name, sellMultiplier, imageFile), properties(std::move(props)), buyPrice(buyPrice) {}

void Star::load(std::istream& is) {
    Item::load(is);
    size_t count;
    is >> count;
    properties.resize(count);
    for (size_t i = 0; i < count; ++i) {
        is >> std::quoted(properties[i]);
    }
}

std::string Star::getType() const {
  return "star";
}

namespace {
  std::shared_ptr<Item> CreateStar() { return std::make_shared<Star>(); }
  const bool registeredStar = ItemFactory::Instance().Register(std::string("Star"), CreateStar);
}

int Star::getBuyPrice() const {
  return buyPrice;
}

int Star::getPropertyCount() const {
  return static_cast<int>(properties.size());
}

int Star::getSellPrice() const {
  return static_cast<int>(getPropertyCount() * PROPERTY_VALUE * sellMultiplier);
}

const std::vector<std::string>& Star::getProperties() const {
  return properties;
}
