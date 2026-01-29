#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

string readToNextTab(const string& row, size_t& startPos) {
  size_t nextTabPos = row.find("\t", startPos);
  size_t substrLen = nextTabPos - startPos;
  string value = row.substr(startPos, substrLen);
  if (nextTabPos != string::npos) {
    startPos = nextTabPos + 1;
  } else {
    startPos = string::npos;
  }
  return value;
}

vector<string> splitByTabs(const string& row) {
  size_t pos = 0;
  vector<string> values;

  while (pos != string::npos) {
    values.push_back(readToNextTab(row, pos));
  }

  return values;
}

void parseBillboardFile(const string& filename, vector<string>& artists) {
  ifstream infile(filename);
  string headerline;
  getline(infile, headerline);  // read and discard headerline

  string row;
  while (getline(infile, row)) {
    vector<string> splitRow = splitByTabs(row);
    artists.push_back(splitRow[1]);
  }
}
