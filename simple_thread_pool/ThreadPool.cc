#include "ThreadPool.h"

#include <unistd.h>

#include <iostream>
ThreadPool::ThreadPool(int thread_num, int task_num)
    : thread_num_(thread_num),
      task_num_(thread_num),
      is_exit_(false),
      task_queue_(task_num) {
  threads_.reserve(thread_num);  // 优化：减少vector扩容
}

ThreadPool::~ThreadPool() {
  if (!is_exit_) {
    stop();
  }
}

void ThreadPool::start() {
  for (int i = 0; i < thread_num_; i++) {
    threads_.emplace_back(std::move(std::make_unique<WorkThread>(
        *this)));  // 注意unique_ptr使用的是对象语义，不能拷贝和赋值，所以此时需要使用move来转化为右值再移动
    threads_[i]->start();
  }
}

void ThreadPool::stop() {
  // 防止队列还有任务时，调用stop，此时会导致任务没有执行完就退出了
  while (!task_queue_.empty()) {
    sleep(1);  // 睡觉防止忙等
  }
  is_exit_ = true;
  task_queue_.wakeup();  // 修复bug，防止stop时，有些线程还在wait上面睡大觉
  for (int i = 0; i < thread_num_; i++) {
    threads_[i]->join();  // stop时等待线程退出
  }
}

// 实现了TaskQueue类之后，add_task和get_task就只是一层封装了
void ThreadPool::add_task(TaskType task) {
  // 判空防止好事者传空任务
  if (task == nullptr) {
    perror("add_task: task is nullptr");
    return;
  }
  task_queue_.add_task(task);
}

TaskType ThreadPool::get_task() { return task_queue_.get_task(); }

void ThreadPool::do_task() {
  // 做任务分两步，先取任务，再处理任务。
  // 注意，这个函数由WorkThread调用，所以是在子线程中执行的
  // 此外，有任务就要一直做，直到线程池退出

  while (!is_exit_) {
    // 严重bug2:使用stop时，这个循环内部可能有线程在get_task内部wait了，导致程序stop仍然卡死，在join等它但它出不来
    TaskType task = get_task();
    if (task) {         // 这里几乎可以保证不为空，但是还是加判空
      task->process();  // 多态
    }
  }
}
