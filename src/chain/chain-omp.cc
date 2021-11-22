#include <omp.h>
#include <iostream>
#include <atomic>

#include "../util/chrono.h"

std::atomic<int> task_counter = 0;
int var1;
int var2;
int var3;
int var4;
int var5;
int var6;
int var7;
int var8;
int N = 10000000;

template<int NumDeps>
void dependency_taskfn();

template<>
void dependency_taskfn<1>()
{
  for (int i = 0; i < N; ++i) {
    #pragma omp task depend(inout: var1)
    {
      if (N < 0) printf("Errrr %d\n", var1);
    }
  }
  #pragma omp taskwait
}

template<>
void dependency_taskfn<2>()
{
  for (int i = 0; i < N; ++i) {
    #pragma omp task depend(inout: var1, var2)
    {
      if (N < 0) printf("Errrr %d %d\n", var1, var2);
    }
  }
  #pragma omp taskwait
}

template<>
void dependency_taskfn<4>()
{
  for (int i = 0; i < N; ++i) {
    #pragma omp task depend(inout: var1, var2, var3, var4)
    {
      if (N < 0) printf("Errrr %d %d %d %d\n", var1, var2, var3, var4);
    }
  }
  #pragma omp taskwait
}

template<>
void dependency_taskfn<8>()
{
  for (int i = 0; i < N; ++i) {
    #pragma omp task depend(inout: var1, var2, var3, var4, var5, var6, var7, var8)
    {
      if (N < 0) printf("Errrr %d %d %d %d %d %d %d %d\n", var1, var2, var3, var4, var5, var6, var7, var8);
    }
  }
  #pragma omp taskwait
}



int main(int argc, char **argv){
  
  int num_deps = 1;
  auto t0 = now();

  if (argc > 1) {
      num_deps = atoi(argv[1]);
  }

#pragma omp parallel master
  {
      switch(num_deps) {
      case 0: /* fall-through */
      case 1: dependency_taskfn<1>(); break;
      case 2: dependency_taskfn<2>(); break;
      case 4: dependency_taskfn<4>(); break;
      case 8: dependency_taskfn<8>(); break;
      default: printf("Unknown number of dependencies: %d\n", num_deps); break;
      }
  }

  auto t1 = now();
  std::cout << "# of tasks = " << N << std::endl;
  std::cout << "time elapsed (microseconds) = " << duration_in_mus(t0, t1) << std::endl;

  return 0;
}

