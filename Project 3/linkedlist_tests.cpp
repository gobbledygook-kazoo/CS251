#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "linkedlist.h"

using namespace std;
using namespace testing;


TEST(LinkedListCore, ConstructorAndBasics) {
    LinkedList<int> ll;
    EXPECT_THAT(ll.size(), Eq(0));
    EXPECT_THAT(ll.empty(), IsTrue());
}

TEST(LinkedListCore, PushPopFront) {
    LinkedList<int> ll;
    ll.push_front(10);
    EXPECT_THAT(ll.size(), Eq(1));
    EXPECT_THAT(ll.at(0), Eq(10));
    
    int val = ll.pop_front();
    EXPECT_THAT(val, Eq(10));
    EXPECT_THAT(ll.size(), Eq(0));
    EXPECT_THAT(ll.empty(), IsTrue());
}

TEST(LinkedListCore, PushBackAndSizeChecks) {
    LinkedList<int> ll;
    ll.push_back(5);
    EXPECT_THAT(ll.size(), Eq(1));
    EXPECT_THAT(ll.at(0), Eq(5));
    
    ll.push_back(15);
    EXPECT_THAT(ll.size(), Eq(2));
    
    ll.pop_back();
    EXPECT_THAT(ll.size(), Eq(1));
    EXPECT_THAT(ll.at(0), Eq(5));
}

TEST(LinkedListCore, AtStrictValueCheck) {
    LinkedList<int> ll;
    ll.push_back(10);
    ll.push_back(20);
    ll.push_back(30);
    

    EXPECT_THAT(ll.at(0), Eq(10));
    EXPECT_THAT(ll.at(1), Eq(20));
    EXPECT_THAT(ll.at(2), Eq(30));
}

TEST(LinkedListCore, AtExceptions) {
    LinkedList<int> ll;
    EXPECT_THROW(ll.at(0), out_of_range);
    ll.push_front(1);
    EXPECT_THROW(ll.at(1), out_of_range);
}

TEST(LinkedListCore, ClearAndDestructor) {
    LinkedList<int> ll;
    ll.push_front(1);
    ll.push_front(2);
    ll.clear();
    EXPECT_THAT(ll.size(), Eq(0));
    EXPECT_THAT(ll.empty(), IsTrue());
}

TEST(LinkedListCore, PopEmptyThrows) {
    LinkedList<int> ll;
    EXPECT_THROW(ll.pop_front(), runtime_error);
    EXPECT_THROW(ll.pop_back(), runtime_error);
}
TEST(LinkedListCore, PopBackOnSingleElement) {
    LinkedList<int> ll;
    ll.push_back(42);
    
    EXPECT_THAT(ll.size(), Eq(1));
    
    int val = ll.pop_back();
    
    EXPECT_THAT(val, Eq(42));
    EXPECT_THAT(ll.size(), Eq(0));
    EXPECT_THAT(ll.empty(), IsTrue());
}
TEST(LinkedListAugmented, CopyConstructorDeepCopy) {
    LinkedList<int> original;
    original.push_back(10);
    original.push_back(20);
    
    LinkedList<int> copy(original);
    ASSERT_THAT(copy.size(), Eq(2));
    EXPECT_THAT(copy.at(0), Eq(10));
    EXPECT_THAT(copy.at(1), Eq(20));
    
    copy.at(0) = 100;
    EXPECT_THAT(original.at(0), Eq(10));
}

TEST(LinkedListAugmented, CopyConstructorEmpty) {
    LinkedList<int> empty_list;
    LinkedList<int> copy(empty_list);
    EXPECT_THAT(copy.size(), Eq(0));
    EXPECT_THAT(copy.empty(), IsTrue());
}

TEST(LinkedListAugmented, AssignmentOperatorSelfAssignment) {
    LinkedList<int> ll;
    ll.push_back(1);
    ll.push_back(2);
    
    LinkedList<int>& llRef = ll;
    ll = llRef; 
    
    ASSERT_THAT(ll.size(), Eq(2));
    EXPECT_THAT(ll.at(0), Eq(1));
    EXPECT_THAT(ll.at(1), Eq(2));
}

TEST(LinkedListAugmented, AssignmentEmptyAndLogic) {
    LinkedList<int> ll1;
    ll1.push_back(5);
    LinkedList<int> ll2;
    
    ll1 = ll2;
    EXPECT_THAT(ll1.size(), Eq(0));
    EXPECT_THAT(ll1.empty(), IsTrue());
}

TEST(LinkedListAugmented, ToStringFormatting) {
    LinkedList<int> ll;
    EXPECT_THAT(ll.to_string(), Eq("[]"));
    
    ll.push_back(1);
    ll.push_back(2);
    EXPECT_THAT(ll.to_string(), Eq("[1, 2]"));
}

TEST(LinkedListAugmented, FindLogic) {
    LinkedList<int> ll;
    ll.push_back(10);
    ll.push_back(20);
    ll.push_back(30);
    
    EXPECT_THAT(ll.find(10), Eq(0));
    EXPECT_THAT(ll.find(30), Eq(2));
    EXPECT_THAT(ll.find(99), Eq((size_t)-1));
}
TEST(LinkedListAugmented, AssignmentNormalCopy) {
    LinkedList<int> source_list;
    source_list.push_back(10);
    source_list.push_back(20);
    source_list.push_back(30);
    
    LinkedList<int> dest_list;
    dest_list.push_back(99);
    
    dest_list = source_list;
    
    ASSERT_THAT(dest_list.size(), Eq(3));
    EXPECT_THAT(dest_list.empty(), IsFalse());
    
    EXPECT_THAT(dest_list.at(0), Eq(10));
    EXPECT_THAT(dest_list.at(1), Eq(20));
    EXPECT_THAT(dest_list.at(2), Eq(30));
    
    dest_list.at(0) = 500;
    EXPECT_THAT(source_list.at(0), Eq(10));
}

TEST(LinkedListExtras, InsertBeforeBadIndex) {
    LinkedList<int> ll;
    ll.push_back(10);

    EXPECT_THROW(ll.insert_before(1, 99), out_of_range); 
    EXPECT_THROW(ll.insert_before(100, 99), out_of_range);
}

TEST(LinkedListExtras, InsertBeforeLastElementAndSize) {
    LinkedList<int> ll;
    ll.push_back(10);
    ll.push_back(20);
    
    ll.insert_before(1, 15);
    
    ASSERT_THAT(ll.size(), Eq(3));
    EXPECT_THAT(ll.at(0), Eq(10));
    EXPECT_THAT(ll.at(1), Eq(15));
    EXPECT_THAT(ll.at(2), Eq(20));
}

TEST(LinkedListExtras, RemoveEveryOtherBasicAndSize) {
    LinkedList<int> ll;
    for (int i = 0; i < 6; i++) {
        ll.push_back(i);
    }
    
    ll.remove_every_other();
    
    ASSERT_THAT(ll.size(), Eq(3));
    
    EXPECT_THAT(ll.at(0), Eq(0));
    EXPECT_THAT(ll.at(1), Eq(2));
    EXPECT_THAT(ll.at(2), Eq(4));
}

TEST(LinkedListExtras, RemoveEveryOtherEmpty) {
    LinkedList<int> ll;

    ll.remove_every_other();
    EXPECT_THAT(ll.size(), Eq(0));
}

TEST(LinkedListExtras, RemoveEveryOtherOddSize) {
    LinkedList<int> ll;
    ll.push_back(10);
    ll.push_back(20);
    ll.push_back(30);
    
    ll.remove_every_other();
    
    ASSERT_THAT(ll.size(), Eq(2));
    EXPECT_THAT(ll.at(0), Eq(10));
    EXPECT_THAT(ll.at(1), Eq(30));
}