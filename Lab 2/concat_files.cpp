#include "concat_files.h"

#include <fstream>
#include <string>
#include <vector>

using namespace std;

vector<string> readlines(const string& fname) {
  vector<string> lines;
  ifstream file(fname);
  if (!file.is_open()) {
    return lines;
  }
  string current_line;
  while (getline(file, current_line)) {
    lines.push_back(current_line);
  }
  return lines;
  return {};
}

void concatFiles(const string& in_fname, const string& out_fname) {
  vector<string> files_to_read = readlines(in_fname);

  ofstream outfile(out_fname);
  for (const string& source_file_name : files_to_read) {
    vector<string> file_contents = readlines(source_file_name);
    for (const string& line : file_contents) {
      outfile << line << endl;
    }
  }
}

int main() {
  // TODO (optional) student: add manual testing code, if desired
  return 0;
}
