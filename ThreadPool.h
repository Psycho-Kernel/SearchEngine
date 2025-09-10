#pragma once
#include <memory>
#include "TaskQueue.h"
#include "Thread.h"
using std::unique_ptr;
using TaskType = Task*;
class ThreadPool {
 public:
  ThreadPool(int thread_num, int task_num);
  ~ThreadPool();
  void start();
  void stop();
  void add_task(TaskType task);
  TaskType get_task();
  void do_task();

 private:
  int thread_num_;
  std::vector<unique_ptr<Thread>> threads_;
  int task_num_;
  TaskQueue task_queue_;
  bool is_exit_;
};
