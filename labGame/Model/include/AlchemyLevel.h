#ifndef LABGAME_ALCHEMY_LEVEL_H
#define LABGAME_ALCHEMY_LEVEL_H

#include <string>

class AlchemyLevel {
private:
  int levelNumber;
  int maxIngredients;
  std::string imageName;

public:
  AlchemyLevel(int level, int maxIngs, const std::string& image);
  
  int getLevelNumber() const;
  int getMaxIngredients() const;
  const std::string& getImageName() const;
};

#endif
