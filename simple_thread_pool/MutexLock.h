// 对pthread_mutex_t对象的包装
#pragma once
#include <pthread.h>
class MutexLock {
 public:
  MutexLock();
  ~MutexLock();
  void lock();
  void unlock();
  bool try_lock();
  pthread_mutex_t* get_mutex_ptr();  // 返回原始锁指针，在Condition中会用到

 private:
  pthread_mutex_t mutex_;
};

// 基于RAII实现的互斥锁的自动释放
class MutexLockGuard {
 public:
  MutexLockGuard(MutexLock& mutex);
  ~MutexLockGuard();

 private:
  MutexLock& mutex_;
};
