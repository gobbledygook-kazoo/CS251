#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstdlib>
#include <fstream>
#include <set>
#include <vector>

#include "include/map_votes.h"
#include "include/vector_votes.h"

using namespace std;
using namespace testing;

namespace {
ifstream input_small("tests_data/votes_small.txt");
ifstream input("votes.txt");
vector<string> votes_small;
set<string> restaurants_small;
vector<string> votes;
set<string> restaurants;

ifstream input_small_map("tests_data/votes_small.txt");
ifstream input_map("votes.txt");
map<string, int> restVotes_small;
map<string, int> restVotes;

TEST(VectorVotes, ProcessVotesSmall) {
  processVotesVector(input_small, votes_small, restaurants_small);
  EXPECT_THAT(votes_small.size(), Eq(12));
  EXPECT_THAT(restaurants_small.size(), Eq(4));
}

TEST(VectorVotes, ProcessVotes) {
  processVotesVector(input, votes, restaurants);
  EXPECT_THAT(votes.size(), Eq(360));
  EXPECT_THAT(restaurants.size(), Eq(13));
}

TEST(VectorVotes, FindMostVotedSmall) {
  string mostVoted = findMostVotedVector(votes_small, restaurants_small);
  EXPECT_THAT(mostVoted, StrEq("McDonalds"));
}

TEST(VectorVotes, FindMostVoted) {
  string mostVoted = findMostVotedVector(votes, restaurants);
  EXPECT_THAT(mostVoted, StrEq("Chilango Mexican Street Food"));
}

TEST(MapVotes, ProcessVotesSmall) {
  processVotesMap(input_small_map, restVotes_small);
  EXPECT_THAT(restVotes_small.size(), Eq(4));
  EXPECT_THAT(restVotes_small["KFC"], Eq(2));
  EXPECT_THAT(restVotes_small["Burger King"], Eq(1));
}

TEST(MapVotes, ProcessVotes) {
  processVotesMap(input_map, restVotes);
  EXPECT_THAT(restVotes.size(), Eq(13));
  EXPECT_THAT(restVotes["Viaggio"], Eq(17));
  EXPECT_THAT(restVotes["Lotus Banh Mi"], Eq(37));
}

TEST(MapVotes, FindMostVotedSmall) {
  string mostVoted = findMostVotedMap(restVotes_small);
  EXPECT_THAT(mostVoted, StrEq("McDonalds"));
}

TEST(MapVotes, FindMostVoted) {
  string mostVoted = findMostVotedMap(restVotes);
  EXPECT_THAT(mostVoted, StrEq("Chilango Mexican Street Food"));
}
}  // namespace