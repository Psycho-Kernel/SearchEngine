#include "Thread.h"

#include <iostream>

#include "ThreadPool.h"
Thread::Thread() : thread_id_(0), is_running_(false) {}

Thread::~Thread() {
  // 子线程析构时， detach 线程
  if (is_running_) {
    pthread_detach(thread_id_);
    is_running_ = false;
  }
}

void Thread::start() {
  int ret = pthread_create(&thread_id_, NULL, thread_func, this);
  if (ret != 0) {
    perror("pthread_create");
  }
  is_running_ = true;
}

void Thread::join() {
  if (is_running_) {
    pthread_join(thread_id_, NULL);
    is_running_ = false;
  }
}

void* Thread::thread_func(void* arg) {
  Thread* thread = static_cast<Thread*>(arg);
  thread->run();
  return NULL;
}

WorkThread::WorkThread(ThreadPool& thread_pool) : thread_pool_(thread_pool) {}

void WorkThread::run() { thread_pool_.do_task(); }
