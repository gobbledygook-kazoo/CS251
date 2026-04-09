#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct TrainStation {
  int id;
  string name;
  int ridership;
};

class MinHeap {
  public:

    void insert(TrainStation val);
    TrainStation remove();
    TrainStation peek();

    int size() {
      return data.size();
    }

  private:
    vector<TrainStation> data;
    
    int _left(int index) {
        return 2*index+1;
    }

    int _right(int index) {
        return 2*index+2;
    }

    int _parent(int index) {
      if (index == 0) {
        return -1;
      }

      return (index - 1)/2;
    }

    void _siftup(int index);
    void _siftdown(int index);
};

