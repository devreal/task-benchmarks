#include <taskflow/taskflow.hpp>  // Taskflow is header-only

#include "../util/chrono.h"

int main(){
  
  tf::Executor executor;
  tf::Taskflow taskflow;

  auto t0 = now();

  tf::Task init = taskflow.emplace([](){if (N < 0) std::cout << N << std::endl;}).name("init");
  tf::Task stop = taskflow.emplace([](){if (N < 0) std::cout << N << std::endl;}).name("stop");

  int N = 10000000;

#ifdef USE_CONDITIONAL_TASK
  // creates a task that increments a counter until target value
  int task_counter = 0;

  tf::Task next = taskflow.emplace(
    [&](){ if (task_counter < N) { ++task_counter; return 0; } else return 1; }
  ).name("next");

  init.precede(next);
  // creates a feedback loop {0: increment, 1: stop}
  next.precede(next, stop);
#else
  tf::Task next = taskflow.emplace(
                              [&](){ if (N < 0) std::cout << N << std::endl; }
                              );
  init.precede(next);
  tf::Task prev = std::move(next);
  for(int t=1; t!=N-2; ++t) {
    tf::Task next = taskflow.emplace(
        [&](){ if (N < 0) std::cout << N << std::endl; }
    );
    prev.precede(next);
    prev = std::move(next);
  }
  prev.precede(stop);
#endif

  executor.run(taskflow).wait();
  auto t1 = now();
  std::cout << "# of tasks = " << N << std::endl;
  std::cout << "time elapsed (microseconds) = " << duration_in_mus(t0, t1) << std::endl;

  return 0;
}

