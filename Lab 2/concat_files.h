#include <string>
#include <vector>

#ifdef COMPILED_FOR_GTEST
#define main original_main
#endif

using namespace std;

/**
 * Reads each line from the given file into a vector, and returns the vector.
 */
vector<string> readlines(const string& fname);

/**
 * `in_fname` is a file that contains a file name on each line. This function
 * reads each line from in_fname, and concatenates the contents in order into
 * the file `out_fname`.
 */
void concatFiles(const string& in_fname, const string& out_fname);
