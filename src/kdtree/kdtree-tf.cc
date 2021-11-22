#include <taskflow/taskflow.hpp>  // Taskflow is header-only

#include "idx.h"

#include "../util/chrono.h"

struct NodeOp {
  Idx idx;

  NodeOp(const Idx& idx) : idx(idx){}

  void operator()(tf::Subflow& subflow) {
    if (idx.l+1<LMAX) {
      subflow.emplace(NodeOp(Idx(idx.l + 1, {{idx.x[0] * 2}})));
      subflow.emplace(NodeOp(Idx(idx.l + 1, {{idx.x[0] * 2 + 1}})));
    }
  }
};

int main(){
  
  tf::Executor executor;
  tf::Taskflow taskflow;

  auto t0 = now();

  tf::Task init = taskflow.emplace([](){ });

  tf::Task down = taskflow.emplace(NodeOp(Idx{}));
  init.precede(down);

  executor.run(taskflow).wait();
  auto t1 = now();
  std::cout << "# of tasks = " << static_cast<uint64_t>(std::pow(2, LMAX)) << std::endl;
  std::cout << "time elapsed (microseconds) = " << duration_in_mus(t0, t1) << std::endl;

  return 0;
}

