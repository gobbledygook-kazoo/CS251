#include <algorithm>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <unordered_set>

using namespace std;

// You can ignore this class! It's just so that we can get "reproducible"
// randomness.
class Random {
 public:
  static mt19937 rng;
  static void seed(int s) {
    Random::rng.seed(s);
  }

  /**
   * Generate a random integer in the range 0 (inclusive) to `max` (exclusive)
   */
  static int randInt(int max) {
    return rng() % (max + 1);
  }
};

std::mt19937 Random::rng;

string shuffle_password(const string& s) {
  string code = to_string(hash<string>{}(s));
  shuffle(code.begin(), code.end(), Random::rng);
  return code;
}

void phase0(string password) {
  string correct = shuffle_password("helloworld");

  if (password != correct) {
    cout << "BOOM!" << endl;
    exit(1);
  }

  cout << "You passed phase 0 with the password <" << password << ">" << endl;
}

void phase1(const string& password) {
  unordered_set<string> animals{2};
  animals.insert("horse");
  animals.insert("dog");
  animals.insert("cat");
  size_t i = 0;
  // This is a C++ "range-based for loop". When used on a container, this
  // iterates over each element of the container, in some order.
  for (const string& animal : animals) {
    if (password[i] != animal[i]) {
      cout << "BOOM!" << endl;
      exit(1);
    }
    ++i;
  }
  cout << "You passed phase 1 with the password <" << password << ">" << endl;
}

void phase2(const string& password) {
  vector<char> chars;
  for (int i = 0; i < 10000; ++i) {
    chars.push_back((char)Random::randInt(126 - 33) + 33);
  }

  bool correct = false;
  for (int i = 0; i < chars.size(); ++i) {
    if (i == 5000 && password.at(i) == chars[i]) {
      correct = true;
    }
  }

  if (!correct) {
    cout << "BOOM!" << endl;
    exit(1);
  }
  cout << "You passed phase 2 with the password <" << password << ">" << endl;
}

int main() {
  Random::seed(1337);
  phase0("06866331481249312070");
  phase1("cog");

  string correct_password(5001, 'a');
  correct_password[5000] = 'M';
  phase2(correct_password);
}
