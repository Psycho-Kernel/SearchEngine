#pragma once
#include <pthread.h>

#include "MutexLock.h"
class Condition {
 public:
  Condition(MutexLock& mutex);
  ~Condition();
  void wait();
  void signal();
  void broadcast();


 private:
  pthread_cond_t cond_;
  MutexLock& mutex_;
};
