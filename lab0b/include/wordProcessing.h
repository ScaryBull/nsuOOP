#ifndef WORDPROCESSING_H
#define WORDPROCESSING_H

#include <iostream>
#include <list>
#include <string>
#include <map>
#include <set>


class WordProcessing {
public:
  std::string nameList;
  std::list<std::string> wordList;
  std::map<std::string, int> countList;
  std::map<int, std::set<std::string>, std::greater<int>> countListReverse;
  int countAllWords = 0;

  WordProcessing(std::string inputNameList="");

  void splitLineIntoWords(std::string& line);
  static void cleanWord(std::string& word);
  void printWord() const;
  void wordCount(std::string& word);
  void printCountWord() const;
  void SortByFreq();
  void printSortCount() const;
};

#endif