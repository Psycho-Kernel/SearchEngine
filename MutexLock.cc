#include "MutexLock.h"

#include <iostream>

MutexLock::MutexLock() {
  int ret = pthread_mutex_init(&mutex_, NULL);
  if (ret != 0) {
    perror("pthread_mutex_init");
  }
}

MutexLock::~MutexLock() {
  int ret = pthread_mutex_destroy(&mutex_);
  if (ret != 0) {
    perror("pthread_mutex_destroy");
  }
}

void MutexLock::lock() {
  int ret = pthread_mutex_lock(&mutex_);
  if (ret != 0) {
    perror("pthread_mutex_lock");
  }
}

void MutexLock::unlock() {
  int ret = pthread_mutex_unlock(&mutex_);
  if (ret != 0) {
    perror("pthread_mutex_unlock");
  }
}

bool MutexLock::try_lock() {
  int ret = pthread_mutex_trylock(&mutex_);
  if (ret != 0) {
    perror("pthread_mutex_trylock");
  }
  return ret == 0;
}

pthread_mutex_t* MutexLock::get_mutex_ptr() { return &mutex_; }

MutexLockGuard::MutexLockGuard(MutexLock& mutex) : mutex_(mutex) {
  mutex_.lock();
}

MutexLockGuard::~MutexLockGuard() { mutex_.unlock(); }
