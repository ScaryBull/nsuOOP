#include "filetxtHandler.h"

FileTxt::FileTxt(std::string inputFileName) {
  fileName = inputFileName;
}

void FileTxt::openFile() {
  fileStream.open(fileName, std::ios::in);
}

void FileTxt::closeFile() {
  fileStream.close();
}

bool FileTxt::readLine(std::string& line) {
  if (std::getline(fileStream, line))
    return true;
  else
    return false;
}

FileTxt::~FileTxt() {
  if (fileStream.is_open())
    fileStream.close();
}
