#pragma once
// #include "ThreadPool.h"
class ThreadPool;
class Task {
 public:
  Task() = default;
  ~Task() = default;
  virtual void process() = 0;  // 纯虚函数
};

class MyTask : public Task {
 public:
  MyTask(ThreadPool& thread_pool) : thread_pool_(thread_pool) {}
  void process() override;

 private:
  ThreadPool& thread_pool_;
};
