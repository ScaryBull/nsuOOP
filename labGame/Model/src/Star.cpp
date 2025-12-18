#include "../include/Star.h"


Star::Star() : Item("", 1.0), properties() {}

Star::Star(const std::string& name, std::vector<std::string> props,
  double sellMultiplier, const std::string& imageFile)
  : Item(name, sellMultiplier, imageFile), properties(std::move(props)) {}

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
  std::shared_ptr<Item> CloneStar(const Item* model) { return std::make_shared<Star>(*static_cast<const Star*>(model)); }
  const bool registeredStar = ItemFactory::Instance().Register(std::string("Star"), CreateStar);
  const bool registeredStarClone = ItemCloneFactory::Instance().Register(std::type_index(typeid(Star)), CloneStar);
}

int Star::getBuyPrice() const {
  return 0;
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

bool Star::matchesRecipe(const std::vector<std::string>& input) const {
  if (input.size() != properties.size())
    return false;

  auto a = properties;
  auto b = input;
  std::sort(a.begin(), a.end());
  std::sort(b.begin(), b.end());
  return a == b;
}
