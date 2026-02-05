#include <fstream>
#include <map>
#include <string>

#ifdef COMPILED_FOR_GTEST
#define main original_main
#endif

using namespace std;

/**
 * Reads each line from the given file that contains a restaurant vote in each
 * line. In the restVotes map, it adds restaurants and associates each with the
 * number of votes that were read from the file.
 */
void processVotesMap(ifstream& file, map<string, int>& restVotes);

/**
 * Computes and returns the restaurant the received the most votes.
 */
string findMostVotedMap(const map<string, int>& restVotes);