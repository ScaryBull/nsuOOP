#ifndef FILECSVHANDLER_H
#define FILECSVHANDLER_H

#include <fstream>
#include <string>
#include <set>

class FileCSVHandler {
  public:
  std::string filecsvName;
  std::ofstream filecsvStream;

  void opencsvFile();
  void closecsvFile();
  void writecsvLine(std::pair<const int, std::set<std::string>>& line, int allCount);

  FileCSVHandler(const std::string& inputFileName = "");
  ~FileCSVHandler();
};

#endif
