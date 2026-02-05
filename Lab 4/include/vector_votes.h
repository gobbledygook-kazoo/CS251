#include <fstream>
#include <set>
#include <string>
#include <vector>

#ifdef COMPILED_FOR_GTEST
#define main original_main
#endif

using namespace std;

/**
 * Reads each line from the given file that contains a restaurant vote in each
 * line. Adds each vote to the votes vector and each seen restaurant to the
 * restaurants set.
 */
void processVotesVector(ifstream& file, vector<string>& votes,
                        set<string>& restaurants);

/**
 * Computes and returns the restaurant the received the most votes.
 */
string findMostVotedVector(const vector<string>& votes,
                           const set<string>& restaurants);