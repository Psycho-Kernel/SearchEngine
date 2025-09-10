#include "Condition.h"

#include <iostream>
Condition::Condition(MutexLock& mutex) : mutex_(mutex) {
  int ret = pthread_cond_init(&cond_, NULL);
  if (ret != 0) {
    perror("pthread_cond_init");
  }
}

Condition::~Condition() {
  int ret = pthread_cond_destroy(&cond_);
  if (ret != 0) {
    perror("pthread_cond_destroy");
  }
}

void Condition::wait() {
  int ret = pthread_cond_wait(&cond_, mutex_.get_mutex_ptr());
  // wait内部分为两部分：
  // 1.上半部分：当前线程放入等待队列，释放锁，睡眠
  // 2.下半部分：从等待队列被唤醒，重新获取锁，成功获取锁后返回
  // 正是因为 pthread_cond_wait 这种“释放锁->等待->醒来->拿回锁”的完整行为，
  // 才引出了使用条件变量的黄金法则：
  // 必须将 pthread_cond_wait 放在一个 while 循环中来检查条件。
  if (ret != 0) {
    perror("pthread_cond_wait");
  }
}

void Condition::signal() {
  int ret = pthread_cond_signal(&cond_);
  if (ret != 0) {
    perror("pthread_cond_signal");
  }
}

void Condition::broadcast() {
  int ret = pthread_cond_broadcast(&cond_);
  if (ret != 0) {
    perror("pthread_cond_broadcast");
  }
}
