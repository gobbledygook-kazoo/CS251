#pragma once

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

using namespace std;

template <typename KeyT, typename ValT>
class HashMap {
 private:
  struct ChainNode {
    const KeyT key;
    ValT value;
    ChainNode* next;

    ChainNode(KeyT key, ValT value) : key(key), value(value), next(nullptr) {
    }

    ChainNode(KeyT key, ValT value, ChainNode* next)
        : key(key), value(value), next(next) {
    }
  };

  ChainNode** data;
  size_t sz;
  size_t capacity;

  // Utility members for begin/next
  ChainNode* curr;
  size_t curr_idx;

 public:
  /**
   * Creates an empty `HashMap` with 10 buckets.
   */
  HashMap() : HashMap(10) {
  }

  /**
   * Creates an empty `HashMap` with `capacity` buckets.
   */
  HashMap(size_t capacity) : sz(0), capacity(capacity), curr(nullptr), curr_idx(0) {
    data = new ChainNode*[capacity]();
  }

  /**
   * Checks if the `HashMap` is empty. Runs in O(1).
   */
  bool empty() const {
    return sz == 0;
  }

  /**
   * Returns the number of mappings in the `HashMap`. Runs in O(1).
   */
  size_t size() const {
    return sz;
  }

  /**
   * Adds the mapping `{key -> value}` to the `HashMap`. If the key already
   * exists, does not update the mapping (like the C++ STL map).
   *
   * Uses the hash value of the key to determine the location in the
   * underlying hash table. Creates exactly one new node; resizes by doubling
   * when the load factor exceeds 1.5.
   *
   * On resize, doesn't create new nodes, but rearranges existing ones.
   *
   * Runs in O(L), where L is the length of the longest chain.
   */
  void insert(KeyT key, ValT value) {
    if (contains(key)) {
      return;
    }


    if ((static_cast<double>(sz + 1) / static_cast<double>(capacity)) > 1.5) {
      size_t old_cap = capacity;
      ChainNode** old_data = data;

      capacity *= 2;
      data = new ChainNode*[capacity]();

      for (size_t i = 0; i < old_cap; ++i) {
        ChainNode* node = old_data[i];
        while (node != nullptr) {
          ChainNode* next_node = node->next;
          
          size_t new_idx = std::hash<KeyT>{}(node->key) % capacity;
          
          node->next = data[new_idx];
          data[new_idx] = node;

          node = next_node;
        }
      }
      delete[] old_data;
    }

    // 3. Insert the new node
    size_t idx = std::hash<KeyT>{}(key) % capacity;
    data[idx] = new ChainNode(key, value, data[idx]);
    sz++;
  }

  /**
   * Return a reference to the value stored for `key` in the map.
   *
   * If key is not present in the map, throw `out_of_range` exception.
   *
   * Runs in O(L), where L is the length of the longest chain.
   */
  ValT& at(const KeyT& key) const {
    size_t idx = std::hash<KeyT>{}(key) % capacity;
    ChainNode* temp = data[idx];
    while (temp != nullptr) {
      if (temp->key == key) {
        return temp->value;
      }
      temp = temp->next;
    }
    throw std::out_of_range("Key not found");
  }

  /**
   * Returns `true` if the key is present in the map, and false otherwise.
   *
   * Runs in O(L), where L is the length of the longest chain.
   */
  bool contains(const KeyT& key) const {
    size_t idx = std::hash<KeyT>{}(key) % capacity;
    ChainNode* temp = data[idx];
    while (temp != nullptr) {
      if (temp->key == key) {
        return true;
      }
      temp = temp->next;
    }
    return false;
  }

  /**
   * Empties the `HashMap`, freeing all nodes. The bucket array may be left
   * alone.
   *
   * Runs in O(N+B), where N is the number of mappings and B is the number of
   * buckets.
   */
  void clear() {
    for (size_t i = 0; i < capacity; ++i) {
      ChainNode* node = data[i];
      while (node != nullptr) {
        ChainNode* next = node->next;
        delete node;
        node = next;
      }
      data[i] = nullptr;
    }
    sz = 0;
  }

  /**
   * Destructor, cleans up the `HashMap`.
   *
   * Runs in O(N+B), where N is the number of mappings and B is the number of
   * buckets.
   */
  ~HashMap() {
    clear();
    delete[] data;
  }

  /**
   * Removes the mapping for the given key from the `HashMap`, and returns the
   * value.
   *
   * Throws `out_of_range` if the key is not present in the map. Creates no new
   * nodes, and does not update the key or value of any existing nodes.
   *
   * Runs in O(L), where L is the length of the longest chain.
   */
  ValT erase(const KeyT& key) {
    size_t idx = std::hash<KeyT>{}(key) % capacity;
    ChainNode* curr_node = data[idx];
    ChainNode* prev = nullptr;

    while (curr_node != nullptr) {
      if (curr_node->key == key) {
        ValT val = curr_node->value;
        if (prev == nullptr) {
          data[idx] = curr_node->next;
        } else {
          prev->next = curr_node->next;
        }
        delete curr_node;
        sz--;
        return val;
      }
      prev = curr_node;
      curr_node = curr_node->next;
    }
    throw std::out_of_range("Key not found");
  }

  /**
   * Copy constructor.
   *
   * Copies the mappings from the provided `HashMap`.
   *
   * Runs in O(N+B), where N is the number of mappings in `other`, and B is the
   * number of buckets.
   */
  HashMap(const HashMap& other) : sz(0), capacity(other.capacity), curr(nullptr), curr_idx(0) {
    data = new ChainNode*[capacity]();
    for (size_t i = 0; i < other.capacity; ++i) {
      ChainNode* node = other.data[i];
      while (node != nullptr) {
        this->insert(node->key, node->value);
        node = node->next;
      }
    }
  }

  /**
   * Assignment operator; `operator=`.
   *
   * Clears this table, and copies the mappings from the provided `HashMap`.
   *
   * Runs in O((N1+B1) + (N2+B2)), where N1 and B1 are the number of mappings
   * and buckets in `this`, and N2 and B2 are the number of mappings and buckets
   * in `other`.
   */
  HashMap& operator=(const HashMap& other) {
    if (this == &other) {
      return *this;
    }
    clear();
    delete[] data;

    capacity = other.capacity;
    data = new ChainNode*[capacity]();
    for (size_t i = 0; i < other.capacity; ++i) {
      ChainNode* node = other.data[i];
      while (node != nullptr) {
        this->insert(node->key, node->value);
        node = node->next;
      }
    }
    return *this;
  }

  // =====================

  /**
   *
   * Checks if the contents of `this` and `other` are equivalent.
   *
   * Two `HashMap` objects are equivalent if they contain the same
   * mappings from key to value. To be equivalent, the two `HashMap`
   * objects need not have the elements saved in the same order
   * inside of the buckets.
   *
   * Runs in worst-case O(B*L*L) time, where B is the maximum number
   * of buckets in either of the `HashMap` objects and L is the length
   * of the  largest chain on any of the buckets.
   *
   */
  bool operator==(const HashMap& other) const {
    if (sz != other.sz) return false;
    for (size_t i = 0; i < capacity; ++i) {
      ChainNode* curr_node = data[i];
      while (curr_node != nullptr) {
        if (!other.contains(curr_node->key)) return false;
        if (other.at(curr_node->key) != curr_node->value) return false;
        
        curr_node = curr_node->next;
      }
    }
    return true;
  }

  /**
   * Resets internal state for an iterative traversal.
   *
   * See `next` for usage details. Modifies nothing except for `curr` and
   * `curr_idx`.
   *
   * Runs in worst-case O(B), where B is the number of buckets.
   */
  void begin() {
    curr_idx = 0;
    curr = nullptr;

    while (curr_idx < capacity) {
      if (data[curr_idx] != nullptr) {
        curr = data[curr_idx];
        break; 
      }
      curr_idx++;
    }
  }

  /**
   * Uses the internal state to return the "next" key and value
   * by reference, and advances the internal state. Returns `true` if the
   * reference parameters were set, and `false` otherwise.
   *
   * Example usage:
   *
   * ```c++
   * HashMap<string, int> hm;
   * hm.begin();
   * string key;
   * int value;
   * while (hm.next(key, val)) {
   *   cout << key << ": " << val << endl;
   * }
   * ```
   *
   * Does not visit the mappings in any defined order.
   *
   * Modifies nothing except for `curr` and `curr_idx`.
   *
   * Runs in worst-case O(B) where B is the number of buckets.
   */
  bool next(KeyT& key, ValT& value) {
    if (curr == nullptr) return false;
    key = curr->key;
    value = curr->value;
    curr = curr->next;

    if (curr == nullptr) {
      curr_idx++;
      while (curr_idx < capacity) {
        if (data[curr_idx] != nullptr) {
          curr = data[curr_idx];
          break;
        }
        curr_idx++;
      }
    }

    return true;
  }

  // ===============================================

  /**
   * Returns a pointer to the underlying memory managed by the `HashMap`.
   * For autograder testing purposes only.
   */
  void* get_data() const {
    return this->data;
  }

  /**
   * Returns the capacity of the underlying memory managed by the `HashMap`. For
   * autograder testing purposes only.
   */
  size_t get_capacity() {
    return this->capacity;
  }
};
