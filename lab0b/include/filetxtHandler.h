#ifndef FILETXTHANDLER_H
#define FILETXTHANDLER_H

#include <fstream>
#include <string>

class FileTxt {
public:
  std::ifstream fileStream;
  std::string fileName;

  FileTxt(std::string inputFileName = "");
  ~FileTxt();

  void openFile();
  void closeFile();
  bool readLine(std::string& line);
};

#endif