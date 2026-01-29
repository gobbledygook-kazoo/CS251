#include <set>
#include <string>
#include <vector>

#ifdef COMPILED_FOR_GTEST
#define main anagrams_main
#endif

using namespace std;

/**
 * Returns a set of the anagrams of `word` that appear in `dictionary`.
 */
set<string> findAnagrams(const string& word, const vector<string>& dictionary);
