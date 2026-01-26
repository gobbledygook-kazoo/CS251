#include <iostream>
#include <string>
#include <vector>

using namespace std;

// TODO student: FIXME!
// Annotate the code on paper, and explain the bugs there first.
// Then, come back here and fix them!
void deduplicate(vector<string>& vec) {
  for (int i = 0; i < vec.size() - 1; i++) {
    if (vec.at(i) == vec.at(i + 1)) {
      vec.erase(vec.begin() + i);
      i--;
    }
  }
}

int main() {
  vector<string> drinks = {
      "coffee", "coffee", "coffee", "hot chocolate", "tea", "tea",
  };

  deduplicate(drinks);

  // drinks should now contain {"coffee", "hot chocolate", "tea"}:
  for (const auto& d : drinks) {
    cout << d << endl;
  }
}
