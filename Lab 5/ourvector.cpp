#include <iostream>

using namespace std;

class ourvector {
 private:
  int* data;
  size_t capacity;
  size_t numElements;

  void resize(size_t newCapacity) {
    if (newCapacity > this->capacity) {
      int* newData = new int[newCapacity];

      for (size_t i = 0; i < this->numElements; ++i) {
        newData[i] = this->data[i];
      }
      delete[] this->data;
      this->data = newData;
      this->capacity = newCapacity;
    }
  }

 public:
  ourvector() {
    this->capacity = 2;
    this->numElements = 0;
    this->data = new int[capacity];
  }
  
  ourvector(const ourvector& other) {
    this->capacity = other.capacity;
    this->numElements = other.numElements;
    this->data = new int[this->capacity];

    for (size_t i = 0; i < this->numElements; ++i) {
        this->data[i] = other.data[i];
    }
}

  void push_back(int value) {
    if (this->numElements == this->capacity) {
      resize(this->capacity + 1);
    }
    this->data[this->numElements] = value;
    this->numElements++;
  }

  int size() {
    return this->numElements;
  }

  int at(size_t index) {
    return this->data[index];
  }

  ~ourvector() {
    delete[] this->data;
  }
};

int sumvec(ourvector v) {
  int sum = 0;
  for (size_t j = 0; j < v.size(); j++) {
    sum += v.at(j);
  }
  return sum;
}

int main() {
  ourvector vec;
  int i = 0;
  while (i <= 10) {
    vec.push_back(i++);
  }
    cout << "summation = " << sumvec(vec) << endl;
}