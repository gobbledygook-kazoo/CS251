#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include "concat_files.h"

using namespace std;

using ::testing::ElementsAreArray;
using ::testing::IsTrue;

namespace {

TEST(ReadLines, EmptyFile) {
  vector<string> expected;

  vector<string> actual = readlines("test_data/empty.txt");
  ASSERT_THAT(actual, ElementsAreArray(expected));
}

TEST(ReadLines, File1) {
  vector<string> expected = {"one_word_line", "line with multiple words",
                             "make sure you're using `getline`, not `>>`!"};

  vector<string> actual = readlines("test_data/file1.txt");
  ASSERT_THAT(actual, ElementsAreArray(expected));
}

TEST(ReadLines, File2) {
  vector<string> expected = {
      "start_of_file_2",
      "how do you handle blank lines?",
      "",
      "you shouldn't need a special case for these",
      "",
      "",
      "blank lines are just empty strings, and getline will read them fine"};

  vector<string> actual = readlines("test_data/file2.txt");
  ASSERT_THAT(actual, ElementsAreArray(expected));
}

const string OUTFILE = "test_data/temp_out.txt";

TEST(ConcatFile, Empty) {
  concatFiles("test_data/write_empty.txt", OUTFILE);

  ifstream actual_f(OUTFILE);
  ASSERT_THAT(actual_f.is_open(), IsTrue())
      << "Error opening file -- does it exist?";

  vector<string> actual = readlines(OUTFILE);
  vector<string> expected;
  EXPECT_THAT(actual, ElementsAreArray(expected));

  actual_f.close();
  filesystem::remove(filesystem::path(OUTFILE));
}

TEST(ConcatFile, LotsOfEmpty) {
  concatFiles("test_data/write_lots_of_empty.txt", OUTFILE);

  ifstream actual_f(OUTFILE);
  ASSERT_THAT(actual_f.is_open(), IsTrue())
      << "Error opening file -- does it exist?";

  vector<string> actual = readlines(OUTFILE);
  vector<string> expected;
  EXPECT_THAT(actual, ElementsAreArray(expected));

  actual_f.close();
  filesystem::remove(filesystem::path(OUTFILE));
}

TEST(ConcatFile, OneFile) {
  concatFiles("test_data/write_one_file.txt", OUTFILE);

  ifstream actual_f(OUTFILE);
  ASSERT_THAT(actual_f.is_open(), IsTrue())
      << "Error opening file -- does it exist?";

  vector<string> actual = readlines(OUTFILE);
  vector<string> expected = {"one_word_line", "line with multiple words",
                             "make sure you're using `getline`, not `>>`!"};
  EXPECT_THAT(actual, ElementsAreArray(expected));

  actual_f.close();
  filesystem::remove(filesystem::path(OUTFILE));
}

TEST(ConcatFile, MultipleFiles) {
  concatFiles("test_data/write_multiple_files.txt", OUTFILE);

  ifstream actual_f(OUTFILE);
  ASSERT_THAT(actual_f.is_open(), IsTrue())
      << "Error opening file -- does it exist?";

  vector<string> actual = readlines(OUTFILE);
  vector<string> expected = {
      "one_word_line",
      "line with multiple words",
      "make sure you're using `getline`, not `>>`!",
      "start_of_file_2",
      "how do you handle blank lines?",
      "",
      "you shouldn't need a special case for these",
      "",
      "",
      "blank lines are just empty strings, and getline will read them fine",
      "one_word_line",
      "line with multiple words",
      "make sure you're using `getline`, not `>>`!"};
  EXPECT_THAT(actual, ElementsAreArray(expected));

  actual_f.close();
  filesystem::remove(filesystem::path(OUTFILE));
}

TEST(ConcatFile, Self) {
  concatFiles("test_data/write_self.txt", OUTFILE);

  ifstream actual_f(OUTFILE);
  ASSERT_THAT(actual_f.is_open(), IsTrue())
      << "Error opening file -- does it exist?";

  vector<string> actual = readlines(OUTFILE);
  vector<string> expected = {
      "test_data/write_self.txt", "test_data/write_self.txt",
      "test_data/write_self.txt", "test_data/write_self.txt"};
  EXPECT_THAT(actual, ElementsAreArray(expected));

  actual_f.close();
  filesystem::remove(filesystem::path(OUTFILE));
}

}  // namespace
