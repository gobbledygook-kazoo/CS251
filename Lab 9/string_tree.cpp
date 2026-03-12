#include "string_tree.h"

#include <fstream>
#include <string>

using namespace std;

StringTree::StringTree(istream& input) {
  // TODO_STUDENT
  // Must be one line long that calls a recursive helper
  this->root = ctor_helper(this->root, input);
}

StringTreeNode* StringTree::ctor_helper(StringTreeNode*& curr, istream& input) {
  string line;
  if (getline(input, line)) {
    string stringForNode = line.substr(3);

    curr = new StringTreeNode(stringForNode);

    if (line.at(0) == 'I') {  // Insert at next level

      ctor_helper(curr->left, input);

      ctor_helper(curr->right, input);
    }
  } else {
    curr = nullptr;
  }
  return curr;
}

void StringTree::serialize(ostream& output) {
  // TODO_STUDENT
  // Must be one line long that calls a recursive helper
  serialize_helper(this->root, output);
}

void StringTree::serialize_helper(StringTreeNode* curr, ostream& output) {
  if (curr == nullptr) {
    return;
  }

  if (curr->left == nullptr && curr->right == nullptr) {
    output << "L: " << curr->data << endl;
  }

  else {
    output << "I: " << curr->data << endl;
    serialize_helper(curr->left, output);
    serialize_helper(curr->right, output);
  }
}

StringTree::~StringTree() {
  // TODO_STUDENT
  // Must be one line long that calls a recursive helper
  dtor_helper(this->root);
}
void StringTree::dtor_helper(StringTreeNode* curr) {
  if (curr == nullptr) {
    return;
  }

  dtor_helper(curr->left);

  dtor_helper(curr->right);

  delete curr;
}