#include "lab2.h"

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int numWordsIn(const string& sentence) {
  stringstream ss(sentence);
  string word;
  int count = 0;
  while (ss >> word) {
    count++;
  }

  return count;
}

int main() {
  string Input;
  cout << "Enter a sentence: ";
  getline(cin, Input);
  int result = numWordsIn(Input);
  cout << result << endl;
  return 0;
}
