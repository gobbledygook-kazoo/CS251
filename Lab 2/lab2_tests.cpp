#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "lab2.h"
#include "test_utils.h"

using ::testing::Eq;
using ::testing::HasSubstr;

namespace {

TEST(NumWordsIn, OneWord) {
  ASSERT_THAT(numWordsIn("word"), Eq(1));
}

TEST(NumWordsIn, MultipleWords) {
  ASSERT_THAT(numWordsIn("This is four words!"), Eq(4));
}

TEST(NumWordsIn, EmptyString) {
  ASSERT_THAT(numWordsIn(""), Eq(0));
}

class Lab1Main : public CaptureCinCout {};

TEST_F(Lab1Main, OneWord) {
  input << "word" << endl;

  original_main();

  string actual_output = output.str();
  ASSERT_THAT(actual_output, HasSubstr("1"));
}

TEST_F(Lab1Main, MultipleWords) {
  input << "This is an input to main!" << endl;

  original_main();

  string actual_output = output.str();
  ASSERT_THAT(actual_output, HasSubstr("6"));
}

TEST_F(Lab1Main, EmptyString) {
  input << "" << endl;

  original_main();

  string actual_output = output.str();
  ASSERT_THAT(actual_output, HasSubstr("0"));
}

// The below tests exercise some weirder cases. These _will_ come up
// on the project, so start thinking about how to handle them!

TEST(NumWordsIn, MultipleSpaces) {
  ASSERT_THAT(
      numWordsIn("This   has   three   spaces   between   every   word"),
      Eq(7));
}

TEST(NumWordsIn, LeadingAndTrailingSpaces) {
  ASSERT_THAT(numWordsIn("    Spaces in front   and at the end   "), Eq(7));
}

TEST(NumWordsIn, EmptyStringWithOnlySpaces) {
  ASSERT_THAT(numWordsIn("    "), Eq(0));
}

}  // namespace
