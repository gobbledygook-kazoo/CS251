#include "heap.h"

#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

void MinHeap::insert(TrainStation val) {
  data.push_back(val);
  _siftup(data.size() - 1);
}


TrainStation MinHeap::remove() {
    if (data.empty()) {
        return {};
    }
    TrainStation minVal = data[0];
    data[0] = data.back();
    data.pop_back();
    if (!data.empty()) {
        _siftdown(0);
    }

    return minVal;

}

TrainStation MinHeap::peek() {
  if (data.empty()) {
    return {};
  }
  return data[0];
}

void MinHeap::_siftup(int index) {
  while (index > 0) {
        int parentIndex = _parent(index);
        if (data[index].ridership < data[parentIndex].ridership) {
            TrainStation temp = data[index];
            data[index] = data[parentIndex];
            data[parentIndex] = temp;
            index = parentIndex;
        } else {
            break;
        }
    }
}

void MinHeap::_siftdown(int index) {
   int size = data.size();
    
    while (true) {
        int left = _left(index);
        int right = _right(index);
        int smallest = index;
        if (left < size && data[left].ridership < data[smallest].ridership) {
            smallest = left;
        }
        if (right < size && data[right].ridership < data[smallest].ridership) {
            smallest = right;
        }
        if (smallest != index) {
            swap(data[index], data[smallest]);
            index = smallest;
        } else {
            break;
        }
    }

}