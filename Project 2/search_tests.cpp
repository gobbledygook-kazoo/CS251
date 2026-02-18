#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "include/search.h"

using namespace std;
using namespace testing;

TEST(cleanToken, punctuationBothEnds) {
  EXPECT_THAT(cleanToken("..hello.."), Eq("hello"));
  EXPECT_THAT(cleanToken("!!!WHaT???"), Eq("what"));
}
