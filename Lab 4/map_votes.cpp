#include "include/map_votes.h"

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>

using namespace std;

void processVotesMap(ifstream& file, map<string, int>& restVotes) {
  // TODO BY STUDENT
  string output;
  while (getline(file, output)) {
    restVotes[output]++;
  }
}

string findMostVotedMap(const map<string, int>& restVotes) {
  // TODO BY STUDENT
  int maxVotes = -1;
  string mostVoted = "";

  for (const auto& entry : restVotes) {
    if (entry.second > maxVotes) {
      maxVotes = entry.second;
      mostVoted = entry.first;
    }
  }
  return mostVoted;
}