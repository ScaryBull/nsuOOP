#include "../include/Ingredient.h"


Ingredient::Ingredient() : Item("", 1.0), buyPrice(0) {}

Ingredient::Ingredient(const std::string& name, const std::string& property, int buyPrice,
  double sellMultiplier, const std::string& imageFile)
    : Item(name, sellMultiplier, imageFile), property(property), buyPrice(buyPrice) {}

void Ingredient::load(std::istream& is) {
    Item::load(is);
    is >> std::quoted(property) >> buyPrice;
}

std::string Ingredient::getType() const {
  return "ingredient";
}

int Ingredient::getBuyPrice() const {
  return buyPrice;
}

int Ingredient::getPropertyCount() const {
  return 1;
}

int Ingredient::getSellPrice() const {
  return static_cast<int>(getPropertyCount() * PROPERTY_VALUE * sellMultiplier);
}

const std::string& Ingredient::getProperty() const {
  return property;
}

const std::vector<std::string>& Ingredient::getProperties() const {
  if (propertiesCache.empty()) {
    propertiesCache.push_back(property);
  }
  return propertiesCache;
}

namespace {
  std::shared_ptr<Item> CreateIngredient() { return std::make_shared<Ingredient>(); }
  std::shared_ptr<Item> CloneIngredient(const Item* model) { return std::make_shared<Ingredient>(*static_cast<const Ingredient*>(model)); }
  const bool registeredIngredient = ItemFactory::Instance().Register(std::string("Ingredient"), CreateIngredient);
  const bool registeredIngredientClone = ItemCloneFactory::Instance().Register(std::type_index(typeid(Ingredient)), CloneIngredient);
}