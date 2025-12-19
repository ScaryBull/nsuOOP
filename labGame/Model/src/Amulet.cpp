#include "../include/Amulet.h"


Amulet::Amulet() : Item("", 1.0), properties() {}

Amulet::Amulet(const std::string& name, std::vector<std::string> props,
  double sellMultiplier, const std::string& imageFile)
    : Item(name, sellMultiplier, imageFile), properties(std::move(props)) {}

void Amulet::load(std::istream& is) {
    Item::load(is);
    size_t count;
    is >> count;
    properties.resize(count);
    for (size_t i = 0; i < count; ++i) {
        is >> std::quoted(properties[i]);
    }
}

std::string Amulet::getType() const {
  return "amulet";
}

namespace {
  std::shared_ptr<Item> CreateAmulet() { return std::make_shared<Amulet>(); }
  std::shared_ptr<Item> CloneAmulet(const Item* model) { return std::make_shared<Amulet>(*static_cast<const Amulet*>(model)); }
  const bool registeredAmulet = ItemFactory::Instance().Register(std::string("Amulet"), CreateAmulet);
  const bool registeredAmuletClone = ItemCloneFactory::Instance().Register(std::type_index(typeid(Amulet)), CloneAmulet);
}

int Amulet::getBuyPrice() const {
  return 0;
}

int Amulet::getPropertyCount() const {
  return static_cast<int>(properties.size());
}

int Amulet::getSellPrice() const {
  return static_cast<int>(getPropertyCount() * PROPERTY_VALUE * sellMultiplier);
}

const std::vector<std::string>& Amulet::getProperties() const {
  return properties;
}

bool Amulet::matchesRecipe(const std::vector<std::string>& input) const {
  if (input.size() != properties.size())
    return false;

  auto a = properties;
  auto b = input;
  std::sort(a.begin(), a.end());
  std::sort(b.begin(), b.end());
  return a == b;
}
