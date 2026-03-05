#pragma once

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

using namespace std;

template <typename T>
class CircVector {
 private:
  T *data;
  size_t vec_size;
  size_t capacity;
  size_t front_idx;


  void resize() {
    size_t new_capacity = capacity * 2;
    T *new_data = new T[new_capacity];
    for (size_t i = 0; i < vec_size; i++) {
        new_data[i] = this->at(i);
    }

    delete[] data;
    data = new_data;
    capacity = new_capacity;
    front_idx = 0;
  }

 public:
  /**
   * Default constructor. Creates an empty `CircVector` with capacity 10.
   */
  CircVector() {
    capacity = 10;
    vec_size = 0;
    front_idx = 0;
    data = new T[capacity];
  }

  /**
   * Creates an empty `CircVector` with given capacity. Capacity must exceed 0.
   */
  CircVector(size_t capacity) {
    this->capacity = capacity;
    this->vec_size = 0;
    this->front_idx = 0;
    this->data = new T[capacity];
  }

  /**
   * Returns whether the `CircVector` is empty (i.e. whether its size is 0).
   */
  bool empty() const {
    return vec_size == 0;
  }

  /**
   * Returns the number of elements in the `CircVector`.
   */
  size_t size() const {
    return vec_size;
  }

  /**
   * Adds the given `T` to the front of the `CircVector`.
   */
  void push_front(T elem) {
    if (vec_size == capacity) {
        resize();
    }
    // Safety formula for size_t wrap-around
    front_idx = (front_idx - 1 + capacity) % capacity;
    data[front_idx] = elem;
    vec_size++;
  }

  /**
   * Adds the given `T` to the back of the `CircVector`.
   */
  void push_back(T elem) {
    if (vec_size == capacity) {
        resize();
    }
    size_t back_idx = (front_idx + vec_size) % capacity;
    data[back_idx] = elem;
    vec_size++;
  }

  /**
   * Removes the element at the front of the `CircVector`.
   */
  T pop_front() {
    if (empty()) {
        throw runtime_error("Empty vector");
    }
    T val = data[front_idx];
    front_idx = (front_idx + 1) % capacity;
    vec_size--;
    return val;
  }

  /**
   * Removes the element at the back of the `CircVector`.
   */
  T pop_back() {
    if (empty()) {
        throw runtime_error("Empty vector");
    }
    T val = this->at(vec_size - 1);
    vec_size--;
    return val;
  }

  /**
   * Removes all elements from the `CircVector`.
   */
  void clear() {
    vec_size = 0;
    front_idx = 0;
  }

  /**
   * Destructor. Clears all allocated memory.
   */
  ~CircVector() {
    delete[] data;
  }

  /**
   * Returns the element at the given index in the `CircVector`.
   */
  T &at(size_t index) const {
    if (index >= vec_size) {
        throw out_of_range("Index out of bounds");
    }
    return data[(front_idx + index) % capacity];
  }

  /**
   * Copy constructor. Creates a deep copy of the given `CircVector`.
   */
  CircVector(const CircVector &other) {
    this->capacity = other.capacity;
    this->vec_size = other.vec_size;
    this->front_idx = 0;
    this->data = new T[this->capacity];

    for (size_t i = 0; i < vec_size; i++) {
        this->data[i] = other.at(i); 
    }
  }

  /**
   * Assignment operator. Sets the current `CircVector` to a deep copy.
   */
  CircVector &operator=(const CircVector &other) {
    if (this == &other) {
        return *this;
    }

    delete[] data;

    this->capacity = other.capacity;
    this->vec_size = other.vec_size;
    this->front_idx = 0; 
    this->data = new T[this->capacity];

    for (size_t i = 0; i < vec_size; i++) {
        this->data[i] = other.at(i);
    }

    return *this;
  }

  /**
   * Converts the `CircVector` to a string. Formatted like `[0, 1, 2, 3, 4]`
   * (without the backticks -- hover the function name to see). Runs in O(N)
   * time.
   */
  string to_string() const {
    stringstream ss;
    ss << "[";
    for (size_t i = 0; i < vec_size; i++) {
        ss << this->at(i);
        if (i < vec_size - 1) {
            ss << ", ";
        }
    }
    ss << "]";
    return ss.str();
}

  /**
   * Searches the `CircVector` for the first matching element, and returns its
   * index in the `CircVector`. If no match is found, returns "-1".
   */
  size_t find(const T &target) {
    for (size_t i = 0; i < vec_size; i++) {
        if (this->at(i) == target) {
            return i;
        }
    }
    return (size_t)-1;
}

  /**
   * Removes the last occurrence of the given `value` from the `CircVector`.
   *
   * For example, if a list was `[0, 1, 2, 1, 0, 1, 8]`, then a call to 
   * `remove_last(1)` should change the list to be [0, 1, 2, 1, 0, 8]`.
   * 
   * If the given value is not found on the `CircVector`, throws an 
   * `invalid_argument` exception.
   */
  void remove_last(const T& value) {
    size_t last_idx = (size_t)-1;
    
    for (size_t i = 0; i < vec_size; i++) {
        if (this->at(i) == value) {
            last_idx = i;
        }
    }

    if (last_idx == (size_t)-1) {
        throw invalid_argument("Value not found");
    }

    for (size_t i = last_idx; i < vec_size - 1; i++) {
        this->at(i) = this->at(i + 1);
    }
    
    vec_size--;
}

  /**
   * Inserts the given `T` as a new element in the `CircVector` before
   * the element currently stored in the given index. If the index is not 
   * a current valid index in the list, throws `out_of_range`.
   */
  void insert_before(size_t index, T elem) {
    if (index >= vec_size) {
        throw out_of_range("Index out of bounds");
    }

    if (vec_size == capacity) {
        resize();
    }


    for (size_t i = vec_size; i > index; i--) {
        size_t dest_idx = (front_idx + i) % capacity;
        size_t src_idx = (front_idx + i - 1) % capacity;
        data[dest_idx] = data[src_idx];
    }

    size_t insert_idx = (front_idx + index) % capacity;
    data[insert_idx] = elem;

    vec_size++;
  }

  /**
   * Remove every other element (alternating) from the
   * `CircVector`, starting at index 1. Must run in O(N). May not reallocate.
   *
   * For example, if a list was `[0, 1, 2, 3, 4]`, removing every other element
   * would change the list to `[0, 2, 4]`.
   */
  void remove_every_other() {
    if (vec_size < 2) {
        return;
    }

    size_t write_idx = 1;

    for (size_t read_idx = 2; read_idx < vec_size; read_idx += 2) {
        this->at(write_idx) = this->at(read_idx);
        write_idx++;
    }
    vec_size = write_idx; 
  }

  /**
   * Returns a pointer to the underlying memory managed by the `CircVec`.
   * For autograder testing purposes only.
   */
  T *get_data() const {
    return this->data;
  }

  /**
   * Returns the capacity of the underlying memory managed by the `CircVec`. For
   * autograder testing purposes only.
   */
  size_t get_capacity() const {
    return this->capacity;
  }
};