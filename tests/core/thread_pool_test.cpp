#include "resource/thread_pool.hpp"

#include <gtest/gtest.h>

using namespace xuzy;

TEST(TaskQueueTest, IncreaseAtomicInteger) {
  static constexpr unsigned int number_of_tasks{1000000};
  std::atomic_uint count{0};
  std::unique_ptr<TaskQueue> task_queue{
      new ThreadPool{CPPHTTPLIB_THREAD_POOL_COUNT}};

  for (unsigned int i = 0; i < number_of_tasks; ++i) {
    auto queued = task_queue->enqueue(
        [&count] { count.fetch_add(1, std::memory_order_relaxed); });
    EXPECT_TRUE(queued);
  }

  EXPECT_NO_THROW(task_queue->shutdown());
  EXPECT_EQ(number_of_tasks, count.load());
}

TEST(TaskQueueTest, IncreaseAtomicIntegerWithQueueLimit) {
  static constexpr unsigned int number_of_tasks{1000000};
  static constexpr unsigned int qlimit{2};
  unsigned int queued_count{0};
  std::atomic_uint count{0};
  std::unique_ptr<TaskQueue> task_queue{
      new ThreadPool{/*num_threads=*/1, qlimit}};

  for (unsigned int i = 0; i < number_of_tasks; ++i) {
    if (task_queue->enqueue(
            [&count] { count.fetch_add(1, std::memory_order_relaxed); })) {
      queued_count++;
    }
  }

  EXPECT_NO_THROW(task_queue->shutdown());
  EXPECT_EQ(queued_count, count.load());
  EXPECT_TRUE(queued_count <= number_of_tasks);
  EXPECT_TRUE(queued_count >= qlimit);
}

TEST(TaskQueueTest, MaxQueuedRequests) {
  static constexpr unsigned int qlimit{3};
  std::unique_ptr<TaskQueue> task_queue{new ThreadPool{1, qlimit}};
  std::condition_variable sem_cv;
  std::mutex sem_mtx;
  int credits = 0;
  bool queued;

  /* Fill up the queue with tasks that will block until we give them credits to
   * complete. */
  for (unsigned int n = 0; n <= qlimit;) {
    queued = task_queue->enqueue([&sem_mtx, &sem_cv, &credits] {
      std::unique_lock<std::mutex> lock(sem_mtx);
      while (credits <= 0) {
        sem_cv.wait(lock);
      }
      /* Consume the credit and signal the test code if they are all gone. */
      if (--credits == 0) {
        sem_cv.notify_one();
      }
    });

    if (n < qlimit) {
      /* The first qlimit enqueues must succeed. */
      EXPECT_TRUE(queued);
    } else {
      /* The last one will succeed only when the worker thread
       * starts and dequeues the first blocking task. Although
       * not necessary for the correctness of this test, we sleep for
       * a short while to avoid busy waiting. */
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    if (queued) {
      n++;
    }
  }

  /* Further enqueues must fail since the queue is full. */
  for (auto i = 0; i < 4; i++) {
    queued = task_queue->enqueue([] {});
    EXPECT_FALSE(queued);
  }

  /* Give the credits to allow the previous tasks to complete. */
  {
    std::unique_lock<std::mutex> lock(sem_mtx);
    credits += qlimit + 1;
  }
  sem_cv.notify_all();

  /* Wait for all the credits to be consumed. */
  {
    std::unique_lock<std::mutex> lock(sem_mtx);
    while (credits > 0) {
      sem_cv.wait(lock);
    }
  }

  /* Check that we are able again to enqueue at least qlimit tasks. */
  for (unsigned int i = 0; i < qlimit; i++) {
    queued = task_queue->enqueue([] {});
    EXPECT_TRUE(queued);
  }

  EXPECT_NO_THROW(task_queue->shutdown());
}