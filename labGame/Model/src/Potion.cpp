#include "../include/Potion.h"
#include "../include/ItemFactories.h"
#include <algorithm>
#include <iomanip>


Potion::Potion() : Item("", 1.0), properties() {}

Potion::Potion(const std::string& name, std::vector<std::string> props,
  double sellMultiplier, const std::string& imageFile)
    : Item(name, sellMultiplier, imageFile), properties(std::move(props)) {}

void Potion::load(std::istream& is) {
    Item::load(is);
    size_t count;
    is >> count;
    properties.resize(count);
    for (size_t i = 0; i < count; ++i) {
        is >> std::quoted(properties[i]);
    }
}

std::string Potion::getType() const {
  return "potion";
}

int Potion::getBuyPrice() const {
  return 0;
}

int Potion::getPropertyCount() const {
  return static_cast<int>(properties.size());
}

int Potion::getSellPrice() const {
  return static_cast<int>(getPropertyCount() * PROPERTY_VALUE * sellMultiplier);
}

const std::vector<std::string>& Potion::getProperties() const {
  return properties;
}

bool Potion::matchesRecipe(const std::vector<std::string>& input) const {
  if (input.size() != properties.size())
    return false;

  auto a = properties;
  auto b = input;
  std::sort(a.begin(), a.end());
  std::sort(b.begin(), b.end());
  return a == b;
}

namespace {
  std::shared_ptr<Item> CreatePotion() { return std::make_shared<Potion>(); }
  std::shared_ptr<Item> ClonePotion(const Item* model) { return std::make_shared<Potion>(*static_cast<const Potion*>(model)); }
  const bool registeredPotion = ItemFactory::Instance().Register(std::string("Potion"), CreatePotion);
  const bool registeredPotionClone = ItemCloneFactory::Instance().Register(std::type_index(typeid(Potion)), ClonePotion);
}