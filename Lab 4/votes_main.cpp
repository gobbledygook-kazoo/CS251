#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "include/map_votes.h"
#include "include/vector_votes.h"

using namespace std;

int main() {
  string filename = "votes.txt";
  ifstream input(filename);
  vector<string> votes;
  set<string> restaurants;

  processVotesVector(input, votes, restaurants);
  string winnerVector = findMostVotedVector(votes, restaurants);

  cout << "Most votes with vector: " << winnerVector << endl;

  ifstream inputMap(filename);
  map<string, int> restVotes;

  processVotesMap(inputMap, restVotes);
  string winnerMap = findMostVotedMap(restVotes);

  cout << "Most votes with map: " << winnerMap << endl;

  return 0;
}