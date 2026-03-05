#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "circvector.h"

using namespace std;
using namespace testing;

TEST(CircVectorCore, ConstructorAndSize) {
  CircVector<int> cv(5);
  EXPECT_THAT(cv.size(), Eq(0));
  EXPECT_THAT(cv.get_capacity(), Eq(5));
}

TEST(CircVectorCore, WrapAroundPushFront) {
  CircVector<int> cv(3);
  cv.push_back(1);
  cv.push_back(2);
  cv.push_front(3);

  EXPECT_THAT(cv.at(0), Eq(3));
  EXPECT_THAT(cv.at(1), Eq(1));
  EXPECT_THAT(cv.at(2), Eq(2));
}

TEST(CircVectorCore, ResizeTrigger) {
  CircVector<int> cv(2);
  cv.push_back(10);
  cv.push_back(20);
  cv.push_back(30);

  EXPECT_THAT(cv.size(), Eq(3));
  EXPECT_THAT(cv.at(0), Eq(10));
  EXPECT_THAT(cv.at(2), Eq(30));
  EXPECT_THAT(cv.get_capacity(), Gt(2));
}

TEST(CircVectorCore, PopEmptyThrows) {
  CircVector<int> cv;
  EXPECT_THROW(cv.pop_front(), runtime_error);
  EXPECT_THROW(cv.pop_back(), runtime_error);
}

TEST(CircVectorCore, WrapAroundPopBack) {
  CircVector<int> cv(4);
  cv.push_front(1);
  cv.push_back(2);
  int val = cv.pop_back();
  EXPECT_THAT(val, Eq(2));
  EXPECT_THAT(cv.size(), Eq(1));
  EXPECT_THAT(cv.at(0), Eq(1));
}

TEST(CircVectorCore, StrictClearAndResize) {
    CircVector<int> cv(2);
    cv.push_front(1);
    cv.push_front(2);
    
    cv.push_front(3); 
    EXPECT_THAT(cv.get_capacity(), Gt(2));
    EXPECT_THAT(cv.size(), Eq(3));
    EXPECT_THAT(cv.at(0), Eq(3));
    
    cv.clear();
    EXPECT_THAT(cv.size(), Eq(0));
}

TEST(CircVectorCore, AtStrictBounds) {
    CircVector<int> cv(10);
    cv.push_back(100);
    EXPECT_THROW(cv.at(1), out_of_range); 
    EXPECT_THROW(cv.at(10), out_of_range);
}

TEST(CircVectorCore, PopFrontSizeConsistency) {
    CircVector<int> cv(5);
    cv.push_back(10);
    cv.pop_front();
    EXPECT_THAT(cv.size(), Eq(0)); 
}

TEST(CircVectorAugmented, CopyConstructorDeepCopy) {
    CircVector<int> original(5);
    original.push_front(10);
    original.push_back(20);
    
    CircVector<int> copy(original);
    ASSERT_THAT(copy.size(), Eq(2));
    EXPECT_THAT(copy.at(0), Eq(10));
    EXPECT_THAT(copy.at(1), Eq(20));
    
    copy.at(0) = 99;
    EXPECT_THAT(original.at(0), Eq(10));
}

TEST(CircVectorAugmented, AssignmentOperatorSelfAssignment) {
    CircVector<int> cv(5);
    cv.push_back(1);
    
    CircVector<int>& cvRef = cv;
    cv = cvRef; 
    
    EXPECT_THAT(cv.size(), Eq(1));
    EXPECT_THAT(cv.at(0), Eq(1));
}

TEST(CircVectorAugmented, AssignmentLogic) {
    CircVector<int> cv1(5);
    cv1.push_back(100);
    CircVector<int> cv2(5);
    cv2.push_back(50);
    
    cv1 = cv2;
    EXPECT_THAT(cv1.size(), Eq(1));
    EXPECT_THAT(cv1.at(0), Eq(50));
}

TEST(CircVectorAugmented, FindWithWraparound) {
    CircVector<int> cv(4);
    cv.push_front(30); 
    cv.push_back(10);
    cv.push_back(20);
    
    EXPECT_THAT(cv.find(30), Eq(0));
    EXPECT_THAT(cv.find(20), Eq(2));
    
    EXPECT_THAT(cv.find(99), Eq((size_t)-1));
}

TEST(CircVectorAugmented, ToStringFormatting) {
    CircVector<int> cv;
    EXPECT_THAT(cv.to_string(), Eq("[]"));
    
    cv.push_back(1);
    cv.push_back(2);
    EXPECT_THAT(cv.to_string(), Eq("[1, 2]"));
}

TEST(CircVectorExtras, InsertBeforeBadIndex) {
    CircVector<int> cv(5);
    cv.push_back(10);
    
    EXPECT_THROW(cv.insert_before(1, 99), out_of_range); 
}

TEST(CircVectorExtras, InsertBeforeLastElementAndSize) {
    CircVector<int> cv(5);
    cv.push_back(10);
    cv.push_back(20);
    
    cv.insert_before(1, 15);
  
    ASSERT_THAT(cv.size(), Eq(3));
    EXPECT_THAT(cv.at(0), Eq(10));
    EXPECT_THAT(cv.at(1), Eq(15));
    EXPECT_THAT(cv.at(2), Eq(20));
}

TEST(CircVectorExtras, InsertBeforeResizeTrigger) {
    CircVector<int> cv(2);
    cv.push_back(10);
    cv.push_back(20);
    

    cv.insert_before(1, 15); 
    
    EXPECT_THAT(cv.size(), Eq(3));
    EXPECT_THAT(cv.get_capacity(), Gt(2));
    EXPECT_THAT(cv.at(1), Eq(15));
}

TEST(CircVectorExtras, RemoveEveryOtherBasicAndSize) {
    CircVector<int> cv(10);
    for (int i = 0; i < 5; i++) {
        cv.push_back(i);
    }
    
    cv.push_front(99);
    cv.pop_front();
    
    cv.remove_every_other();
    
    ASSERT_THAT(cv.size(), Eq(3));
    
    EXPECT_THAT(cv.at(0), Eq(0));
    EXPECT_THAT(cv.at(1), Eq(2));
    EXPECT_THAT(cv.at(2), Eq(4));
}