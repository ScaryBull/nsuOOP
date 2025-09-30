#include "wordProcessing.h"

WordProcessing::WordProcessing(std::string inputNameList) {
  nameList = inputNameList;
}

void WordProcessing::splitLineIntoWords(std::string& line) {
  std::string word;
  for (int i = 0; i < line.length(); i++) {
    if (!isalnum(line[i])) {
      if (!word.empty()) {
        wordList.push_back(word);
        wordCount(word);
        countAllWords++;
      }
      word = "";
    } else {
      word += tolower(line[i]);
    }
  }
  if (!word.empty()) {
    wordList.push_back(word);
    wordCount(word);
    countAllWords++;
  }
}

void WordProcessing::printWord() const {
  for (const auto& word : wordList) {
    std::cout << word << std::endl;
  }
}

void WordProcessing::wordCount(std::string& word) {
  if (countList.count(word)) {
    countList[word]++;
  } else {
    countList[word] = 1;
  }
}

void WordProcessing::printCountWord() const {
  for (const auto& pair : countList) {
    std::cout << pair.first << "\t" << pair.second << std::endl;
  }
}

void WordProcessing::SortByFreq() {
  std::string word;
  for (auto& pair : countList) {
    countListReverse[pair.second].insert(pair.first);
  }
}

void WordProcessing::printSortCount() const {
  for (const auto& pair : countListReverse) {
    std::cout << pair.first << ": ";
    for (const auto& word : pair.second) {
      std::cout << word << " ";
    }
    std::cout << std::endl;
  }
}