#include "../include/Item.h"


Item::Item(const std::string& name, double sellMultiplier, std::string imageFile)
    : name(name), sellMultiplier(sellMultiplier), imageFile(imageFile) {}

void Item::load(std::istream& is) {
    is >> std::quoted(name) >> sellMultiplier >> std::quoted(imageFile);
}

const std::string& Item::getName() const {
  return name;
}

double Item::getSellMultiplier() const {
  return sellMultiplier;
}

const std::string& Item::getImageFileName() const {
  return imageFile;
}

const std::string& Item::getImageFile() const {
  return imageFile;
}