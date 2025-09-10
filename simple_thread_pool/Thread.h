#pragma once
#include <pthread.h>

class ThreadPool;
// 注意，Thread有run纯虚函数，所以Thread不能实例化
class Thread {
 public:
  Thread();
  ~Thread();
  void start();
  void join();

 private:
  // 线程入口函数
  static void* thread_func(void* arg);
  // 线程执行任务，入口函数会调用run
  virtual void run() = 0;

 private:
  pthread_t thread_id_;
  bool is_running_;
};

class WorkThread : public Thread {
 public:
  WorkThread(ThreadPool& thread_pool);
  void run() override;

 private:
  ThreadPool& thread_pool_;
};
