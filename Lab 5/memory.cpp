#include <iostream>

using namespace std;

void f1() {
  int arr[5] = {1, 2, 3, 4, 5};
  for (int i = 0; i <= 5; ++i) {
    cout << arr[i] << endl;
  }
}

void f2() {
  int* ptr = nullptr;
  *ptr = 42;
}

void f3() {
  int* arr = new int[5];
  delete[] arr;
  delete[] arr;
}

void f4() {
  int* p = new int;
  *p = 5;
}

void f5() {
  int* ptr;
  *ptr = 42;
}

int main() {
  // Call the functions here.
  f1();
  // f2();
   //f3();
  //f4();
  // f5();
}