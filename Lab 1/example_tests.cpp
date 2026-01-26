#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "example.h"

using testing::DoubleEq;
using testing::StrEq;

TEST(Message, CorrectMessage) {
  ASSERT_THAT(message("Sparky"), StrEq("Hello, Sparky!"));
}

TEST(Average, OneNumber) {
  ASSERT_THAT(average({5}), DoubleEq(5.0));
}

TEST(Average, TwoNumbers) {
  ASSERT_THAT(average({5, 6}), DoubleEq(5.5));
}

TEST(Average, ManyNumbers) {
  ASSERT_THAT(average({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}), DoubleEq(5.5));
}
