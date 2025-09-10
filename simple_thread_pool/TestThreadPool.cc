#include <iostream>

#include "Task.h"
#include "ThreadPool.h"
using std::cout;
using std::endl;
int main() {
  ThreadPool thread_pool(4, 10);
  unique_ptr<Task> ptask(new MyTask(thread_pool));
  thread_pool.start();
  int count = 20;
  while (count--) {
    thread_pool.add_task(ptask.get());
    cout << "count = " << count << endl;
  }
  thread_pool.stop();
  return 0;
}
