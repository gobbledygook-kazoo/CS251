#include <string>
#include <vector>

using namespace std;

#ifdef COMPILED_FOR_GTEST
#define main original_main
#endif

string message(const string& name);
double average(vector<int> v);
