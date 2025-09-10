#include "TaskQueue.h"
TaskQueue::TaskQueue(int queue_size)
    : capacity_(queue_size),
      task_queue_(),
      mutex_(),
      not_empty_(mutex_),
      not_full_(mutex_),
      is_exit_(false) {}

TaskQueue::~TaskQueue() {}

bool TaskQueue::empty() { return task_queue_.empty(); }

bool TaskQueue::full() { return task_queue_.size() == capacity_; }

void TaskQueue::add_task(TaskType task) {
  MutexLockGuard lock(mutex_);  // 使用基于RAII思想的锁
  // 构造时自动加锁，析构时自动解锁，作用域限于这个函数内

  // 如果任务队列已满，则等待在not_full_条件上
  while (full()) {
    not_full_.wait();
  }
  // 走到这说明已经拿到锁，且任务队列不满
  task_queue_.push(task);
  not_empty_.signal();  // 添加任务后，不要忘记唤醒等待在not_empty_条件上的线程
}

// 代码不再赘述，和add_task雷同
TaskType TaskQueue::get_task() {
  MutexLockGuard lock(mutex_);
  while (empty() && !is_exit_) {
    not_empty_.wait();
  }
  if (!is_exit_) {
    TaskType task = task_queue_.front();
    task_queue_.pop();
    not_full_.signal();
    return task;
  } else {
    return nullptr;
  }
}
void TaskQueue::wakeup() {
  is_exit_ = true;
  not_empty_.broadcast();
}
