#pragma once

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

using namespace std;

template <typename T>
class LinkedList {
 private:
  class Node {
   public:
    T data;
    Node *next;

    Node(T data) {
      this->data = data;
      this->next = nullptr;
    }

    Node(T data, Node *next) {
      this->data = data;
      this->next = next;
    }
  };

  size_t list_size;
  Node *list_front;

 public:
  /**
   * Default constructor. Creates an empty `LinkedList`.
   */
  LinkedList() {
    list_front = nullptr; //
    list_size = 0;       //
  }

  /**
   * Returns whether the `LinkedList` is empty.
   */
  bool empty() const {
    return list_size == 0; // One statement
  }

  /**
   * Returns the number of elements in the `LinkedList`.
   */
  size_t size() const {
    return list_size; // One statement
  }

  /**
   * Adds the given `T` to the front of the `LinkedList`.
   */
  void push_front(T data) {
    Node* newNode = new Node(data, list_front); //
    list_front = newNode;
    list_size++; //
  }

  /**
   * Adds the given `T` to the back of the `LinkedList`.
   */
  void push_back(T data) {
    Node* newNode = new Node(data);
    if (empty()) {
        list_front = newNode; //
    } else {
        Node* curr = list_front;
        while (curr->next != nullptr) { // Traverse to end
            curr = curr->next;
        }
        curr->next = newNode;
    }
    list_size++; //
  }

  /**
   * Removes the element at the front of the `LinkedList`.
   */
  T pop_front() {
    if (empty()) {
        throw runtime_error("List is empty"); //
    }
    Node* temp = list_front;
    T data = temp->data;
    list_front = list_front->next;
    delete temp; // Memory safety
    list_size--;
    return data;
  }

  /**
   * Removes the element at the back of the `LinkedList`.
   */
  T pop_back() {
    if (empty()) {
        throw runtime_error("List is empty"); //
    }
    T data;
    if (list_size == 1) {
        data = list_front->data;
        delete list_front;
        list_front = nullptr;
    } else {
        Node* curr = list_front;
        while (curr->next->next != nullptr) {
            curr = curr->next;
        }
        data = curr->next->data;
        delete curr->next; //
        curr->next = nullptr;
    }
    list_size--;
    return data;
  }

  /**
   * Empties the `LinkedList` and releasing all allocated memory.
   */
  void clear() {
    while (!empty()) {
        pop_front(); //
    }
  }

  /**
   * Destructor. Clears all allocated memory.
   */
  ~LinkedList() {
    clear(); //
  }

  /**
   * Returns the element at the given index in the `LinkedList`.
   */
  T& at(size_t index) const {
    if (index >= list_size) {
        throw out_of_range("Index out of bounds"); //
    }
    Node* curr = list_front;
    for (size_t i = 0; i < index; i++) {
        curr = curr->next;
    }
    return curr->data; //
  }
  /**
   * Copy constructor. Creates a deep copy of the given `LinkedList`.
   *
   * Must run in O(N) time.
   */
  LinkedList(const LinkedList &other) {
    list_front = nullptr; 
    list_size = 0;       

    if (other.list_front == nullptr) {
        return; 
    }

    list_front = new Node(other.list_front->data);
    list_size++;

    Node* tail = list_front;
    Node* curr = other.list_front->next;

    while (curr != nullptr) {
        tail->next = new Node(curr->data);
        tail = tail->next;
        list_size++;
        curr = curr->next;
    }
  }
  /**
   * Assignment operator. Sets the current `LinkedList` to a deep copy of the
   * given `LinkedList`.
   *
   * Must run in O(N) time.
   */
  LinkedList &operator=(const LinkedList &other) {
    if (this == &other) {
        return *this;
    }

    this->clear(); 

    if (other.list_front != nullptr) {
        list_front = new Node(other.list_front->data);
        list_size++;
        
        Node* tail = list_front;
        Node* curr = other.list_front->next;
        
        while (curr != nullptr) {
            tail->next = new Node(curr->data);
            tail = tail->next;
            list_size++;
            curr = curr->next;
        }
    }
    return *this;
  }

  /**
   * Converts the `LinkedList` to a string. Formatted like `[0, 1, 2, 3, 4]`
   * (without the backticks -- hover the function name to see). Runs in O(N)
   * time.
   */
  string to_string() const {
    stringstream ss;
    ss << "[";
    Node* curr = list_front;
    while (curr != nullptr) {
        ss << curr->data;
        if (curr->next != nullptr) {
            ss << ", ";
        }
        curr = curr->next;
    }
    ss << "]";
    return ss.str();
}

  /**
   * Searches the `LinkedList` for the first matching element, and returns its
   * index. If no match is found, returns "-1".
   */
  size_t find(const T &target) {
    Node* curr = list_front;
    for (size_t i = 0; i < list_size; i++) {
        if (curr->data == target) {
            return i; // Catch: Returns the wrong index
        }
        curr = curr->next;
    }
    return (size_t)-1;
}

  /**
   * Removes the last occurrence of the given value from the `LinkedList`.
   *
   * For example, if a list was `[0, 1, 2, 1, 0, 1, 8]`, then a call to 
   * `remove_last(1)` should change the list to be [0, 1, 2, 1, 0, 8]`.
   * 
   * If the given value is not found on the `LinkedList`, throws an 
   * `invalid_argument` exception.
   */
  void remove_last(const T& value) {
    size_t last_idx = (size_t)-1;
    Node* curr = list_front;
    
    for (size_t i = 0; i < list_size; i++) {
        if (curr->data == value) {
            last_idx = i;
        }
        curr = curr->next;
    }

    if (last_idx == (size_t)-1) {
        throw invalid_argument("Value not found");
    }

    if (last_idx == 0) {
        pop_front(); //
    } else if (last_idx == list_size - 1) {
        pop_back();  //
    } else {
        Node* prev = list_front;
        for (size_t i = 0; i < last_idx - 1; i++) {
            prev = prev->next;
        }
        Node* toDelete = prev->next;
        prev->next = toDelete->next;
        delete toDelete; //
        list_size--;
    }
}

  /**
   * Inserts the given `T` as a new element in the `LinkedList` before
   * the element currently stored in the given index. If the index is not 
   * a current valid index in the list, throws `out_of_range`.
   */
  void insert_before(size_t index, T elem) {
    if (index >= list_size) {
        throw out_of_range("Index out of bounds");
    }

    if (index == 0) {
        push_front(elem);
        return;
    }

    Node* prev = list_front;
    for (size_t i = 0; i < index - 1; i++) {
        prev = prev->next;
    }

    Node* newNode = new Node(elem, prev->next);
    
    prev->next = newNode;
    
    list_size++; 
  }

  /**
   * Remove every other element (alternating) from the
   * `LinkedList`, starting at index 1. Must run in O(N).
   *
   * For example, if a list was `[0, 1, 2, 3, 4]`, removing every other element
   * would change the list to `[0, 2, 4]`.
   */
  void remove_every_other() {
    // Catch: buggy remove_every_other when the list is empty
    if (list_size < 2) {
        return; // Nothing to remove if size is 0 or 1
    }

    Node* curr = list_front;

    // Iterate through the list. curr is the node we KEEP. 
    // curr->next is the node we REMOVE.
    // Catch: buggy remove_every_other when the list has an odd size
    while (curr != nullptr && curr->next != nullptr) {
        Node* toDelete = curr->next;
        
        curr->next = toDelete->next;
        
        delete toDelete;
        
        list_size--;
        
        curr = curr->next; 
    }
  }
  /**
   * Returns a pointer to the node at the front of the `LinkedList`. For
   * autograder testing purposes only.
   */
  void *front() const {
    return this->list_front;
  }
};