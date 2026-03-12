#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <fstream>
#include <sstream>
#include <tuple>

#include "string_tree.h"

using namespace testing;
using namespace std;

namespace {

// Why write a helper instead of a matcher?
// I want to maintain a "message".

bool compareTrees(StringTreeNode* actual,
                  StringTreeNode* expected, int depth,
                  ostream& stream) {
  string padding(2 * depth, ' ');
  if ((actual == nullptr) != (expected == nullptr)) {
    stream << "<INCORRECT NODE>" << endl;
    if (actual == nullptr) {
      stream << padding << "Expected to not be nullptr, but got nullptr"
             << endl;
    } else {
      stream << padding << "Expected to be nullptr, but got a non-null pointer"
             << endl;
    }
    return false;
  }

  if (actual == nullptr && expected == nullptr) {
    return true;
  }

  if (actual->data != expected->data) {
    stream << "<INCORRECT DATA>" << endl;
    stream << padding << "Expected data \"" << expected->data << "\"" << endl
           << padding << "Got data  \"" << actual->data << "\"" << endl;
    return false;
  }

  stream << "\"" << actual->data << "\"" << endl;

  // Could be on one line, but this gives better tracebacks
  if (actual->left || expected->left) {
    stream << padding << "  (L) " << endl;
  }
  bool leftOk = compareTrees(actual->left, expected->left, depth + 1, stream);
  if (!leftOk) {
    return false;
  }

  if (actual->right || expected->right) {
    stream << padding << "  (R) " << endl;
  }
  bool rightOK = compareTrees(actual->left, expected->left, depth + 1, stream);
  return rightOK;
}

TEST(StringTree, Empty) {
  StringTree expected(nullptr);

  ifstream in("data/empty.txt");
  StringTree actual(in);

  ostringstream out;
  bool compRes = compareTrees(actual.root, expected.root, 0, out);
  EXPECT_TRUE(compRes) << "Checking tree built from data/empty.txt..." << endl
                       << out.str();
}

TEST(StringTreeCtor, Fruit) {
  // clang-format off
  StringTree expected(
    new StringTreeNode("apple",
      new StringTreeNode("orange",
        new StringTreeNode("pineapple"),
        new StringTreeNode("kiwi")
      ),
      new StringTreeNode("lime",
        new StringTreeNode("lemon",
          new StringTreeNode("watermelon"),
          new StringTreeNode("strawberry")
        ),
        new StringTreeNode("mango")
      )
    )
  );
  // clang-format on

  ifstream in("data/fruit.txt");
  StringTree actual(in);

  ostringstream out;
  bool compRes = compareTrees(actual.root, expected.root, 0, out);
  EXPECT_TRUE(compRes) << out.str();
}

TEST(StringTreeCtor, Soda) {
  // clang-format off
  StringTree expected(
    new StringTreeNode("Dr. Pepper",
      new StringTreeNode("Sprite",
        new StringTreeNode("Mountain Dew",
          new StringTreeNode("Coke"),
          new StringTreeNode("Root Beer")
        ),
        new StringTreeNode("Ginger Ale")
      ),
      new StringTreeNode("Fanta",
        new StringTreeNode("Sunkist"),
        new StringTreeNode("Pepsi")
      )
    )
  );
  // clang-format on

  ifstream in("data/soda.txt");
  StringTree actual(in);

  ostringstream out;
  bool compRes = compareTrees(actual.root, expected.root, 0, out);
  EXPECT_TRUE(compRes) << out.str();
}

TEST(StringTreeSerialize, SerializeFruit) {
  ifstream in("data/fruit.txt");

  stringstream ss;
  string line;
  while (getline(in, line)) {
    ss << line << endl;
  }
  string contents = ss.str();

  stringstream in_ss(contents);
  StringTree tree(in_ss);

  ostringstream actualOut;
  tree.serialize(actualOut);
  EXPECT_THAT(actualOut.str(), StrEq(contents));
}

TEST(StringTreeSerialize, SerializeSoda) {
  ifstream in("data/soda.txt");

  stringstream ss;
  string line;
  while (getline(in, line)) {
    ss << line << endl;
  }
  string contents = ss.str();

  stringstream in_ss(contents);
  StringTree tree(in_ss);

  ostringstream actualOut;
  tree.serialize(actualOut);
  EXPECT_THAT(actualOut.str(), StrEq(contents));
}

}  // namespace
