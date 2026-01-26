#include <iostream>
#include <vector>

#include "example.h"

using namespace std;

string message(const string& name) {
  return "hello, " + name;
}

double average(vector<int> v) {
  int sum = 0;
  for (size_t i = 0; i < v.size(); i++) {
    sum += v[i];
  }

  return sum / v.size();
}

int main() {
  string msg = message("Sparky");
  cout << msg << endl;

  vector<int> elems = {7, 7, 9, 9};
  cout << "Average: " << average(elems) << endl;

  return 0;
}
