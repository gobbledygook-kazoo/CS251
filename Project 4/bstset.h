#pragma once

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

using namespace std;

template <typename T>
class BSTSet {
 private:
  struct BSTNode {
    // These two members should never change after initialization!
    const T value;
    BSTNode* parent;
    BSTNode* left;
    BSTNode* right;

    // This is fancy constructor syntax
    // Don't worry too much about it -- it's just necesssary for the const
    // member.
    BSTNode(T value, BSTNode* parent)
        : value(value), parent(parent), left(nullptr), right(nullptr) {
    }
  };

  BSTNode* root;
  size_t sz;

  // Utility pointer for begin and next.
  BSTNode* curr;

  // TODO_STUDENT: add private helper function definitions here
  void _clear(BSTNode* node) {
    if (node == nullptr) return;
    _clear(node->left);
    _clear(node->right);
    delete node;
  }

bool _contains(BSTNode* node, const T& value) const {
    if (node == nullptr) return false;
    if (value == node->value) return true;
    if (value < node->value) return _contains(node->left, value);
    return _contains(node->right, value);
  }

void _insert(BSTNode*& node, BSTNode* parent, T value) {
    if (node == nullptr) {
      node = new BSTNode(value, parent);
      sz++;
      return;
    }
    if (value < node->value) {
      _insert(node->left, node, value);
    } else if (value > node->value) {
      _insert(node->right, node, value);
    }
  }

void _to_string(BSTNode* node, ostringstream& oss, bool& first) const {
    if (node == nullptr) return;
    _to_string(node->left, oss, first);
    if (!first) {
      oss << ", ";
    }
    oss << node->value;
    first = false;
    _to_string(node->right, oss, first);
  }

BSTNode* _copy(BSTNode* node, BSTNode* new_parent) {
    if (node == nullptr) return nullptr;
    BSTNode* newNode = new BSTNode(node->value, new_parent);
    newNode->left = _copy(node->left, newNode);
    newNode->right = _copy(node->right, newNode);
    return newNode;
  }

BSTNode* _findMin(BSTNode* node) const {
    if (node == nullptr) return nullptr;
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

BSTNode* _findNode(BSTNode* node, const T& value) const {
    if (node == nullptr || node->value == value) return node;
    if (value < node->value) return _findNode(node->left, value);
    return _findNode(node->right, value);
}

void _replaceNode(BSTNode* u, BSTNode* v) {
    if (u->parent == nullptr) {
        root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    if (v != nullptr) {
        v->parent = u->parent;
    }
}

 public:
  /**
   * Creates an empty `BSTSet`. Runs in O(1).
   */
  BSTSet(): root(nullptr), sz(0), curr(nullptr) {
    // TODO_STUDENT
  }

  /**
   * Checks if the `BSTSet` is empty. Runs in O(1).
   */
  bool empty() const {
    return sz == 0;
  }

  /**
   * Returns the number of elements in the `BSTSet`. Runs in O(1).
   */
  size_t size() const {
    return sz;
    return 0;
  }

  /**
   * Adds the `value` to the `BSTSet`. If the value already
   * exists in the set, does not do anything.
   *
   * Uses the `value` to determine the location in the underlying BST.
   * Creates exactly one new node.
   *
   * Runs in O(H), where H is the height of the tree.
   */
  void insert(T value) {
    _insert(root, nullptr, value);
  }

  /**
   * Returns `true` if the `value` is present in the set,
   * and `false` otherwise.
   *
   * Runs in O(H), where H is the height of the tree.
   */
  bool contains(const T& value) const {
    return _contains(root, value);
  }

  /**
   * Empties the `BSTSet`, freeing all memory it controls.
   *
   * Runs in O(N), where N is the number of elements in the set.
   */
  void clear() {
    _clear(root);
    root = nullptr;
    sz = 0;
  }

  /**
   * Destructor, cleans up the `BSTSet`.
   *
   * Runs in O(N), where N is the number of elements in the set.
   */
  ~BSTSet() {
    clear();
  }

  /**
   * Converts the `BSTSet` to a string representation, with the values
   * in-order by value.
   *
   * Example:
   *
   * c++
   * BSTSet<string> names;
   * names.insert("Gwen");
   * names.insert("Jen");
   * names.insert("Ben");
   * names.insert("Sven");
   *
   * Calling `names.to_string()` would return the following
   * string: `{Ben, Gwen, Jen, Sven}`
   *
   * Runs in O(N), where N is the number of elements.
   */
  string to_string() const {
    ostringstream oss;
    oss << "{";
    bool first = true;
    _to_string(root, oss, first);
    oss << "}";
    return oss.str();
  }

  /**
   * Copy constructor.
   *
   * Copies the elements from the provided `BSTSet`.
   * The internal tree structure must be copied exactly.
   *
   * Runs in O(N), where N is the number of elements in `other`.
   */
  BSTSet(const BSTSet& other) {
    sz = other.sz;
    curr = nullptr;
    root = _copy(other.root, nullptr);
  }

  /**
   * Assignment operator; `operator=`.
   *
   * Clears this tree, and copies the elements from the provided `BSTSet`.
   * The internal tree structure must be copied exactly.
   *
   * Runs in O(N + O), where N is the number of elements in `this`, and O is
   * the number of elements in `other`.
   */
  BSTSet& operator=(const BSTSet& other) {
    if (this != &other) {
      clear();
      sz = other.sz;
      root = _copy(other.root, nullptr);
    }
    return *this;
  }

  // =========================================================================

  /**
   * Locates the smallest element in the BSTSet, and removes it
   * from the `BSTSet`. If the `BSTSet` is empty, throws a
   * `runtime_error`.
   *
   * Creates no new nodes, and does not update any
   * existing nodes.
   *
   * Runs in O(H), where H is the height of the tree.
   */
  T remove_min() {
    // TODO_STUDENT
    if (root == nullptr) {
      throw runtime_error("BSTSet is empty");
    }

    BSTNode* minNode = _findMin(root);
    T minValue = minNode->value;

    BSTNode* child = minNode->right;
    BSTNode* par = minNode->parent;
    if (par == nullptr) {
      root = child;
    } else {
      par->left = child;
    }

    if (child != nullptr) {
      child->parent = par;
    }

    delete minNode;
    sz--;
    return minValue;
  }

  /**
   * Resets internal state for an iterative inorder traversal.
   *
   * See `next` for usage details. Modifies nothing except for `curr`.
   *
   * O(H), where H is the maximum height of the tree.
   */
  void begin() {
    curr = _findMin(root);
  }

  /**
   * Uses the internal state to return the next in-order element
   * by reference, and advances the internal state. Returns `true`
   * if the reference was set, and `false` otherwise.
   *
   * Example usage:
   *
   * c++
   * BSTSet<string> bst;
   * bst.begin();
   * string value;
   * while (bst.next(val)) {
   *   cout << val << endl;
   * }
   * ```
   *
   * Modifies nothing except for `curr`.
   *
   * Runs in worst-case O(H) where H is the height of the tree.
   */
  bool next(T& value) {
    if (curr == nullptr) {
      return false;
    }
    value = curr->value;

    if (curr->right != nullptr) {
      curr = _findMin(curr->right);
    } else {
      BSTNode* p = curr->parent;
      while (p != nullptr && curr == p->right) {
        curr = p;
        p = p->parent;
      }
      curr = p;
    }

    return true;
  }

  // =========================================================================

  /**
   * Removes the given element from the `BSTSet`, and returns the
   * value.
   *
   * Throws `out_of_range` if the value is not present in the set.
   * Creates no new nodes, and does not update the value of any
   * existing nodes.
   *
   * Runs in worst-case O(H) where H is the height of the tree.
   */
  T erase(const T& value) {
    BSTNode* z = _findNode(root, value);
    if (z == nullptr) {
        throw out_of_range("Value not in set");
    }

    T returnValue = z->value;

    if (z->left == nullptr) {
        _replaceNode(z, z->right);
    } else if (z->right == nullptr) {
        _replaceNode(z, z->left);
    } 
    else {
        BSTNode* y = _findMin(z->right);

        if (y->parent != z) {
            _replaceNode(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }


        _replaceNode(z, y);
        y->left = z->left;
        y->left->parent = y;
    }

    delete z;
    sz--;
    return returnValue;
  }

  // =========================================================================

  /**
   * Returns a pointer to the root node of the `BSTSet`. For autograder testing
   * purposes only.
   */
  void* getRoot() const {
    return this->root;
  }
};
