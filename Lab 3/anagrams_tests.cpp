#include <gtest/gtest.h>

#include <fstream>
#include <string>
#include <utility>
#include <vector>

#include "anagrams.h"

using namespace std;

using ::testing::Test;
using ::testing::Values;
using ::testing::WithParamInterface;

class WordAnagramTest : public Test,
                        public WithParamInterface<tuple<string, set<string>>> {
 protected:
  static void SetUpTestSuite() {
    ifstream infile("dictionary.txt");
    string line;
    while (getline(infile, line)) {
      dictionary.push_back(line);
    }
  }

  static vector<string> dictionary;
};

// ??? why ???
// https://stackoverflow.com/a/42763774
vector<string> WordAnagramTest::dictionary;

TEST_P(WordAnagramTest, Case) {
  auto& [word, expected_anagrams] = GetParam();

  set<string> actual_anagrams = findAnagrams(word, dictionary);

  ASSERT_EQ(actual_anagrams, expected_anagrams);
}

INSTANTIATE_TEST_SUITE_P(
    , WordAnagramTest,
    Values(make_tuple("listen", set<string>{"listen", "silent", "tinsel"}),
           make_tuple("meat", set<string>{"mate", "meat", "tame", "team"}),
           make_tuple("", set<string>{}),
           make_tuple("magikarp", set<string>{})));
