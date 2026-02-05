#include "include/vector_votes.h"

#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <vector>

using namespace std;

void processVotesVector(ifstream& file, vector<string>& votes,
                        set<string>& restaurants) {
  // TODO BY STUDENT
  string output;
  while ((getline(file, output))) {
    votes.push_back(output);
    restaurants.insert(output);
  }
}

string findMostVotedVector(const vector<string>& votes,
                           const set<string>& restaurants) {
  // TODO BY STUDENT
  int maxVotes = -1;
  string highlyVoted = "";

  for (const string& rest : restaurants) {
    int count = 0;
    for (const string& vote : votes) {
      if (vote == rest) {
        count++;
      }
      if (count > maxVotes) {
        maxVotes = count;
        highlyVoted = rest;
      }
    }
  }
  return highlyVoted;
}
