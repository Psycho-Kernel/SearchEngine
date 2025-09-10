#include "Task.h"

#include <iostream>
using std::cout;
using std::endl;
void MyTask::process() {
  ::srand(::clock());
  int number = rand() % 100;
  cout << "MyTask::process :" << number << endl;
}
