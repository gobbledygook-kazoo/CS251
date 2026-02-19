#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "include/search.h"

using namespace std;
using namespace testing;

TEST(cleanToken, punctuationBothEnds) {
  EXPECT_THAT(cleanToken("..hello.."), Eq("hello"));
  EXPECT_THAT(cleanToken("!!!WHaT???"), Eq("what"));
}

TEST(CleanToken, punctuationMiddle) {
    EXPECT_THAT(cleanToken("dog-tired"), Eq("dog-tired"));
    EXPECT_THAT(cleanToken("can't"), Eq("can't"));
}

TEST(CleanToken, punctuationMiddleAndStart) {
    EXPECT_THAT(cleanToken("...globe-matter"), Eq("globe-matter"));
}

TEST(CleanToken, punctuationMiddleAndEnd) {
    EXPECT_THAT(cleanToken("gluten-free!!!"), Eq("gluten-free"));
}

TEST(CleanToken, noLetters) {
    EXPECT_THAT(cleanToken("56554"), Eq(""));
    EXPECT_THAT(cleanToken("!!!123!!!"), Eq(""));
    EXPECT_THAT(cleanToken("..."), Eq(""));
}

TEST(GatherTokens, basicCase) {
    string text = "to be or not to be";
    set<string> expected = {"to", "be", "or", "not"};
    EXPECT_THAT(gatherTokens(text), ContainerEq(expected));
}

TEST(GatherTokens, frontSpace) {
    string text = "   hello world";
    set<string> expected = {"hello", "world"};
    EXPECT_THAT(gatherTokens(text), ContainerEq(expected));
}

TEST(GatherTokens, endSpace) {
    string text = "hello world   ";
    set<string> expected = {"hello", "world"};
    EXPECT_THAT(gatherTokens(text), ContainerEq(expected));
}

TEST(GatherTokens, middleSpace) {
    string text = "hello     world";
    set<string> expected = {"hello", "world"};
    EXPECT_THAT(gatherTokens(text), ContainerEq(expected));
}

TEST(GatherTokens, cleanCase) {
    string text = "...apple, BANANA! 12345 apple";
    set<string> expected = {"apple", "banana"};
    EXPECT_THAT(gatherTokens(text), ContainerEq(expected));
}


TEST(BuildIndex, tinyFile) {
    string filename = "data/tiny.txt";
    map<string, set<string>> studentIndex;
    int studentNumProcessed = buildIndex(filename, studentIndex);

    set<string> fishUrls = {"www.dr.seuss.net", "www.shoppinglist.com"};
    EXPECT_THAT(studentIndex["fish"], ContainerEq(fishUrls));
    
    set<string> blueUrls = {"www.dr.seuss.net", "www.rainbow.org"};
    EXPECT_THAT(studentIndex["blue"], ContainerEq(blueUrls));
}

TEST(BuildIndex, notFound) {
    map<string, set<string>> index;
    int pages = buildIndex("nonexistent.txt", index);
    
    EXPECT_THAT(pages, Eq(0));
    EXPECT_THAT(index.size(), Eq(0));
}

TEST(FindQueryMatches, FirstTermMissing) {
    map<string, set<string>> mockIndex = {
        {"hello", {"example.com", "uic.edu"}},
        {"there", {"example.com"}}
    };
    set<string> expectedEmpty = {};
    set<string> expectedHello = {"example.com", "uic.edu"};

    EXPECT_THAT(findQueryMatches(mockIndex, "missing"), ContainerEq(expectedEmpty));

    EXPECT_THAT(findQueryMatches(mockIndex, "missing hello"), ContainerEq(expectedHello));

    EXPECT_THAT(findQueryMatches(mockIndex, "missing +hello"), ContainerEq(expectedEmpty));

    EXPECT_THAT(findQueryMatches(mockIndex, "missing -hello"), ContainerEq(expectedEmpty));
}

TEST(FindQueryMatches, LaterTermMissing) {
    map<string, set<string>> mockIndex = {
        {"hello", {"example.com", "uic.edu"}}
    };
    set<string> expectedHello = {"example.com", "uic.edu"};
    set<string> expectedEmpty = {};

    EXPECT_THAT(findQueryMatches(mockIndex, "hello missing"), ContainerEq(expectedHello));

    EXPECT_THAT(findQueryMatches(mockIndex, "hello +missing"), ContainerEq(expectedEmpty));

    EXPECT_THAT(findQueryMatches(mockIndex, "hello -missing"), ContainerEq(expectedHello));
}
