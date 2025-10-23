#include <string>

#include "filecsvHandler.h"
#include "filetxtHandler.h"
#include "wordProcessing.h"

int main(int argc, char* argv[]) {
  if (argc != 3) {
    return 1;
  }

  std::string inputFile = argv[1];
  std::string outputFile = argv[2];
  std::string line;

  FileTxt fileTxt(inputFile);
  FileCSVHandler fileCsv(outputFile);
  WordProcessing wordProcessing;

  fileTxt.openFile();
  fileCsv.opencsvFile();

  while (fileTxt.readLine(line)) {
    wordProcessing.splitLineIntoWords(line);
  }
  wordProcessing.SortByFreq();
  for (auto& pair : wordProcessing.countListReverse) {
    fileCsv.writecsvLine(pair, wordProcessing.countAllWords);
  }

  fileTxt.closeFile();
  fileCsv.closecsvFile();

  return 0;
}
