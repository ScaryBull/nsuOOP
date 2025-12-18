#include "../include/AlchemyLevel.h"

AlchemyLevel::AlchemyLevel(int level, int maxIngs, const std::string& image)
    : levelNumber(level), maxIngredients(maxIngs), imageName(image) {}

int AlchemyLevel::getLevelNumber() const {
  return levelNumber;
}

int AlchemyLevel::getMaxIngredients() const {
  return maxIngredients;
}

const std::string& AlchemyLevel::getImageName() const {
  return imageName;
}
