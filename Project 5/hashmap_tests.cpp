#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <random>

#include "hashmap.h"

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

TEST(HashMapCore, InitializationAndBasicProperties) {
    HashMap<int, int> hm;
    EXPECT_TRUE(hm.empty());
    EXPECT_EQ(hm.size(), 0);
    hm.insert(1, 100);
    EXPECT_FALSE(hm.empty());
    EXPECT_EQ(hm.size(), 1);
}

TEST(HashMapCore, InsertDoesNotOverwrite) {
    HashMap<int, string> hm;
    hm.insert(1, "Original");
    hm.insert(1, "New");
    
    EXPECT_EQ(hm.size(), 1);
    EXPECT_EQ(hm.at(1), "Original");
}

TEST(HashMapCore, CollisionHandling) {
    HashMap<int, int> hm(10);
    hm.insert(5, 50);
    hm.insert(15, 150);
    hm.insert(25, 250);
    
    EXPECT_EQ(hm.size(), 3);
    EXPECT_TRUE(hm.contains(5));
    EXPECT_TRUE(hm.contains(15));
    EXPECT_TRUE(hm.contains(25));
    
    EXPECT_EQ(hm.at(25), 250);
    EXPECT_EQ(hm.at(5), 50);
}

TEST(HashMapCore, ResizeTrigger) {
    HashMap<int, int> hm(10);
    for (int i = 0; i < 15; ++i) {
        hm.insert(i, i * 10);
    }
    EXPECT_EQ(hm.get_capacity(), 10);
    
    hm.insert(15, 150);
    EXPECT_EQ(hm.get_capacity(), 20);
    EXPECT_EQ(hm.size(), 16);
    EXPECT_EQ(hm.at(0), 0);
    EXPECT_EQ(hm.at(15), 150);
}

TEST(HashMapCore, EraseAndExceptions) {
    HashMap<int, int> hm(10);
    hm.insert(5, 50);
    hm.insert(15, 150);
    hm.insert(25, 250);
    
    // Erase middle node
    EXPECT_EQ(hm.erase(15), 150);
    EXPECT_EQ(hm.size(), 2);
    EXPECT_FALSE(hm.contains(15));
    
    EXPECT_EQ(hm.at(5), 50);
    EXPECT_EQ(hm.at(25), 250);
    
    // Test exception
    EXPECT_THROW(hm.at(99), std::out_of_range);
    EXPECT_THROW(hm.erase(99), std::out_of_range);
}

TEST(HashMapCore, CopyAndAssignment) {
    HashMap<int, int> hm1;
    hm1.insert(1, 10);
    hm1.insert(2, 20);
    
    HashMap<int, int> hm2(hm1);
    EXPECT_EQ(hm2.size(), 2);
    EXPECT_TRUE(hm2.contains(1));
    EXPECT_TRUE(hm2.contains(2));
    EXPECT_EQ(hm2.at(1), 10);
    
    hm2.insert(3, 30);
    EXPECT_FALSE(hm1.contains(3));
    
    HashMap<int, int> hm3;
    hm3.insert(100, 1000);
    hm3 = hm1;
    
    EXPECT_EQ(hm3.size(), 2);
    EXPECT_TRUE(hm3.contains(1));
    EXPECT_TRUE(hm3.contains(2));
    EXPECT_EQ(hm3.at(2), 20);
    EXPECT_FALSE(hm3.contains(100));
    
    HashMap<int, int>& self = hm1;
    hm1 = self; 
    EXPECT_EQ(hm1.size(), 2);
    EXPECT_TRUE(hm1.contains(1));
}

TEST(HashMapCore, CopyEmptyMap) {
    HashMap<int, int> empty_src;
    
    HashMap<int, int> empty_copy(empty_src);
    EXPECT_EQ(empty_copy.size(), 0);
    EXPECT_TRUE(empty_copy.empty());
    
    HashMap<int, int> target;
    target.insert(5, 50);
    target = empty_src;
    EXPECT_EQ(target.size(), 0);
    EXPECT_TRUE(target.empty());
}

TEST(HashMapCore, ClearFunctionality) {
    HashMap<int, int> hm;
    hm.insert(1, 10);
    hm.insert(2, 20);
    hm.clear();
    
    EXPECT_EQ(hm.size(), 0);
    EXPECT_TRUE(hm.empty());
    EXPECT_FALSE(hm.contains(1));
}

template <typename K, typename V>
vector<pair<K, V>> collect_elements(HashMap<K, V>& hm) {
    vector<pair<K, V>> elements;
    hm.begin();
    K key;
    V val;
    while (hm.next(key, val)) {
        elements.push_back({key, val});
    }
    return elements;
}

TEST(HashMapAugmented, EqualityChecksValues) {
    HashMap<int, string> hm1;
    HashMap<int, string> hm2;

    hm1.insert(1, "Apple");
    hm2.insert(1, "Banana");

    EXPECT_FALSE(hm1 == hm2);
    
    hm2.erase(1);
    hm2.insert(1, "Apple");
    EXPECT_TRUE(hm1 == hm2);
}

TEST(HashMapAugmented, EqualityChecksSize) {
    HashMap<int, int> hm1;
    HashMap<int, int> hm2;

    hm1.insert(1, 10);
    hm1.insert(2, 20);
    hm2.insert(1, 10);

    EXPECT_FALSE(hm1 == hm2);
    EXPECT_FALSE(hm2 == hm1);
}

TEST(HashMapAugmented, IterationEmptyTable) {
    HashMap<int, int> hm;
    hm.begin();
    int k, v;
    

    EXPECT_FALSE(hm.next(k, v));
}

TEST(HashMapAugmented, IterationWithCollisions) {
    HashMap<int, int> hm(5);
    
    hm.insert(0, 100);
    hm.insert(5, 500);
    hm.insert(10, 1000);
    hm.insert(1, 10);
    
    vector<pair<int, int>> expected = {{0, 100}, {5, 500}, {10, 1000}, {1, 10}};
    vector<pair<int, int>> actual = collect_elements(hm);

    EXPECT_EQ(actual.size(), 4);
    EXPECT_THAT(actual, UnorderedElementsAreArray(expected));
}

TEST(HashMapAugmented, EqualityDifferentCapacities) {
    HashMap<int, int> hm1(10);
    HashMap<int, int> hm2(20);

    for(int i = 0; i < 5; ++i) {
        hm1.insert(i, i * 10);
        hm2.insert(i, i * 10);
    }

    EXPECT_TRUE(hm1 == hm2);
}

}  // namespace
