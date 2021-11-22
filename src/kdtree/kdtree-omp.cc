
#include "idx.h"

#include <atomic>
#include <iostream>

#include "../util/chrono.h"

static void taskfn(const Idx& idx)
{
  {
    if (idx.l+1 < LMAX) {
      #pragma omp task firstprivate(idx)
        taskfn(Idx(idx.l + 1, {{idx.x[0] * 2}}));
      #pragma omp task firstprivate(idx)
        taskfn(Idx(idx.l + 1, {{idx.x[0] * 2+1}}));
    }
  }
}

int main(){
  
  auto t0 = now();

#pragma omp parallel master
  {
    #pragma omp task
    {
      taskfn(Idx{});
    }
    #pragma omp taskwait
  }

  auto t1 = now();
  std::cout << "# of tasks = " << static_cast<uint64_t>(std::pow(2, LMAX)) << std::endl;
  std::cout << "time elapsed (microseconds) = " << duration_in_mus(t0, t1) << std::endl;

  return 0;
}

