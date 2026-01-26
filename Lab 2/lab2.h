#include <string>

#ifdef COMPILED_FOR_GTEST
#define main original_main
#endif

using namespace std;

/**
 * Returns the number of words in `sentence`.
 */
int numWordsIn(const string& sentence);
