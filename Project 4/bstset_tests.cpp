#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <random>

#include "bstset.h"

using namespace std;
using namespace testing;

namespace {

class Random {
 private:
  static mt19937 rng;

 public:
  static void seed(int s) {
    Random::rng.seed(s);
  }

  /**
   * Generate a random integer in the range 0 (inclusive) to `max` (exclusive)
   */
  static int randInt(int max) {
    // Even though mt19937 is standardized, STL distributions aren't!
    // So, unfortunately, even though this is biased, we're forced to
    // do something like this. Technically uint32_fast_t isn't...
    // totally consistent across platforms? But within reason it works.
    return rng() % (max + 1);
  }
};

std::mt19937 Random::rng;

TEST(BSTSetCore, basicSize) {
  BSTSet<int> bst;
    EXPECT_TRUE(bst.empty());
    EXPECT_EQ(bst.size(), 0);
    
    bst.insert(10);
    EXPECT_FALSE(bst.empty());
    EXPECT_EQ(bst.size(), 1);
    
    bst.insert(10);
    EXPECT_EQ(bst.size(), 1);
}

TEST(BSTSetCore, copyAssign) {
    BSTSet<int> original;
    original.insert(20);
    
    BSTSet<int> assigned;
    assigned = original;
    
    BSTSet<int>* ptr = &assigned;
    assigned = *ptr; 
    
    EXPECT_EQ(assigned.size(), 1);
}
TEST(BSTSetCore, format) {
    BSTSet<string> names;
    names.insert("Gwen");
    names.insert("Ben");
    EXPECT_THAT(names.to_string(), Eq("{Ben, Gwen}"));
}
TEST(BSTSetCore, emptyCopy) {
    BSTSet<int> empty_set;
    BSTSet<int> copy(empty_set);
    EXPECT_EQ(copy.size(), 0);
    EXPECT_TRUE(copy.empty());

    BSTSet<int> full_set;
    full_set.insert(10);
    full_set = empty_set;
    EXPECT_EQ(full_set.size(), 0);
}
TEST(BSTSetCore, containsChecks) {
    BSTSet<int> bst;
    bst.insert(50);
    bst.insert(25);
    bst.insert(75);

    EXPECT_TRUE(bst.contains(50));
    EXPECT_TRUE(bst.contains(25));
    EXPECT_FALSE(bst.contains(100));
    EXPECT_FALSE(bst.contains(60));
}
TEST(BSTSetCore, random) {
    Random::seed(42);
    BSTSet<int> bst;
    for(int i = 0; i < 100; ++i) {
        bst.insert(Random::randInt(1000));
    }
    bst.clear();
    EXPECT_EQ(bst.size(), 0);
}

TEST(BSTSetCore, CopyConstructorDeepCheck) {
    BSTSet<int> original;
    original.insert(50);
    original.insert(25);
    original.insert(75);

    BSTSet<int> copy(original); 
    
    EXPECT_EQ(copy.size(), 3);
    EXPECT_FALSE(copy.empty());
    EXPECT_TRUE(copy.contains(50));
    EXPECT_TRUE(copy.contains(25));
    EXPECT_TRUE(copy.contains(75));
}

TEST(BSTSetCore, AssignmentDeepCheck) {
    BSTSet<int> original;
    original.insert(100);
    original.insert(50);

    BSTSet<int> assigned;
    assigned.insert(1);
    
    assigned = original; 
    
    EXPECT_EQ(assigned.size(), 2);
    EXPECT_FALSE(assigned.empty());
    EXPECT_TRUE(assigned.contains(100));
    EXPECT_TRUE(assigned.contains(50));
    EXPECT_FALSE(assigned.contains(1));
}
TEST(BSTSetAugmented, RemoveMinBasic) {
    BSTSet<int> bst;
    
    EXPECT_THROW(bst.remove_min(), runtime_error);

    bst.insert(10);
    bst.insert(20);
    EXPECT_EQ(bst.remove_min(), 10);
    EXPECT_EQ(bst.size(), 1);
    EXPECT_TRUE(bst.contains(20));
    EXPECT_FALSE(bst.contains(10));

    bst.clear();
    bst.insert(20);
    bst.insert(10);
    bst.insert(15);

    EXPECT_EQ(bst.remove_min(), 10);
    EXPECT_EQ(bst.size(), 2);
    EXPECT_TRUE(bst.contains(15));
    EXPECT_TRUE(bst.contains(20));
}
TEST(BSTSetAugmented, IterationStandard) {
    BSTSet<int> bst;
    
    bst.begin();
    int val;
    EXPECT_FALSE(bst.next(val));

    vector<int> data = {50, 25, 75, 10, 30, 60, 80};
    for (int d : data) bst.insert(d);

    vector<int> expected = {10, 25, 30, 50, 60, 75, 80};
    vector<int> actual;

    bst.begin();
    while (bst.next(val)) {
        actual.push_back(val);
    }

    EXPECT_EQ(actual, expected);
    
    EXPECT_FALSE(bst.next(val));
}
TEST(BSTSetAugmented, IterationRandomStress) {
    Random::seed(123);
    BSTSet<int> bst;
    set<int> truth;
    
    for(int i = 0; i < 20; ++i) {
        int r = Random::randInt(1000);
        bst.insert(r);
        truth.insert(r);
    }

    bst.begin();
    int val;
    auto it = truth.begin();
    while (bst.next(val)) {
        EXPECT_EQ(val, *it);
        it++;
    }
    EXPECT_EQ(it, truth.end());
}

TEST(BSTSetErase, EraseComplex) {
    BSTSet<int> bst;
    EXPECT_THROW(bst.erase(100), out_of_range);

    vector<int> data = {50, 25, 75, 60, 80, 65};
    for (int d : data) bst.insert(d);
    EXPECT_EQ(bst.erase(50), 50);
    EXPECT_EQ(bst.size(), 5);
    EXPECT_FALSE(bst.contains(50));
    EXPECT_TRUE(bst.contains(65));
}
TEST(BSTSetErase, EraseRootNoChildren) {
    BSTSet<int> bst;
    bst.insert(10);
    
    // Test erasing the only node in the tree
    EXPECT_EQ(bst.erase(10), 10);
    
    // Verify tree is now completely empty
    EXPECT_EQ(bst.size(), 0);
    EXPECT_TRUE(bst.empty());
    EXPECT_FALSE(bst.contains(10));
    
    // Ensure we can still interact with it
    bst.insert(20);
    EXPECT_EQ(bst.size(), 1);
    EXPECT_TRUE(bst.contains(20));
}

TEST(BSTSetErase, EraseTwoChildrenComplex) {
    BSTSet<int> bst;
    vector<int> data = {50, 25, 75, 60, 80, 65};
    for (int d : data) bst.insert(d);

    EXPECT_EQ(bst.erase(50), 50);
    EXPECT_TRUE(bst.contains(65));
    EXPECT_TRUE(bst.contains(60));
    EXPECT_EQ(bst.size(), 5);
}

}  // namespace
