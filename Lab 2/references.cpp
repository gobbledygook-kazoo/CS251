#include <iostream>
#include <vector>

using namespace std;

void printVec(vector<int>& v) {
  for (int i : v) {
    cout << i << " ";
  }
  cout << endl;
}

void maui(vector<int> v) {
  for (size_t i = 0; i < v.size(); i++) {
    v[i] += 2;
  }
}

void sina(vector<int>& v) {
  for (size_t i = 0; i < v.size(); i++) {
    v[i] += 2;
  }
}

void moana(vector<int>& v) {
  for (auto i : v) {
    i += 2;
  }
}

void heihei(vector<int>& v) {
  for (auto& i : v) {
    i += 2;
  }
}

vector<int> teFiti(vector<int>& v) {
  vector<int> result;
  for (int i : v) {
    result.push_back(i + 2);
  }
  return result;
}

int main() {
  vector<int> v = {4, 2, 5};

  string s;
  cout << "Hit <ENTER> to continue." << endl;
  getline(cin, s);

  maui(v);
  printVec(v);

  cout << "Hit <ENTER> to continue." << endl;
  getline(cin, s);

  sina(v);
  printVec(v);

  cout << "Hit <ENTER> to continue." << endl;
  getline(cin, s);

  moana(v);
  printVec(v);

  cout << "Hit <ENTER> to continue." << endl;
  getline(cin, s);

  heihei(v);
  printVec(v);

  cout << "Hit <ENTER> to continue." << endl;
  getline(cin, s);

  teFiti(v);
  printVec(v);

  return 0;
}
