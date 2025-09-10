#pragma once
#include <queue>

#include "Condition.h"
#include "MutexLock.h"
using std::queue;
#include "Task.h"
using TaskType = Task*;  // using放在类外面，因为其实现也会用到这个类型
class TaskQueue {
 public:
  TaskQueue(int queue_size);
  ~TaskQueue();
  bool empty();
  bool full();
  void add_task(TaskType task);
  TaskType get_task();
  void wakeup();

 private:
  int capacity_;
  queue<TaskType> task_queue_;
  MutexLock mutex_;      // 用于保护任务队列的互斥锁
  Condition not_empty_;  // 条件变量，用于等待任务队列不为空
  Condition not_full_;   // 条件变量，用于等待任务队列不满
  bool is_exit_;         // 用于修复bug:即线程池stop时，还有线程在wait上面睡觉
};
