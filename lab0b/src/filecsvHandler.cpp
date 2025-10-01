#include "filecsvHandler.h"


FileCSVHandler::FileCSVHandler(const std::string& inputFileName) {
  filecsvName = inputFileName;
 }

void FileCSVHandler::opencsvFile() {
  filecsvStream.open(filecsvName, std::ios::out);
}

void FileCSVHandler::closecsvFile() {
  filecsvStream.close();
}

void FileCSVHandler::writecsvLine(std::pair<const int, std::set<std::string>>& line, int allCount) {
  for (const auto& word : line.second) {
    double percent = static_cast<double>(line.first) * 100.0 / allCount;
    filecsvStream << word << "," << line.first << "," << percent << "%\n";
  }
}


FileCSVHandler::~FileCSVHandler() {
  if (filecsvStream.is_open())
    filecsvStream.close();
}