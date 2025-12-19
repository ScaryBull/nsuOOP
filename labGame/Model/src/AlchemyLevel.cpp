#include "../include/AlchemyLevel.h"

AlchemyLevel::AlchemyLevel(int level, int maxIngs, const std::string& image, int price)
  : levelNumber(level), maxIngredients(maxIngs), imageName(image), price(price) {}

int AlchemyLevel::getLevelNumber() const {
  return levelNumber;
}

int AlchemyLevel::getMaxIngredients() const {
  return maxIngredients;
}

const std::string& AlchemyLevel::getImageName() const {
  return imageName;
}

int AlchemyLevel::getPrice() const {
  return price;
}
