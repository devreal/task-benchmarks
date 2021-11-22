//#define TTG_USE_USER_TERMDET 1
#include "ttg.h"

#include "../util/chrono.h"

using namespace ttg;

template <int num_flows>
auto make_ttg();

// flows task ids via values
template <>
auto make_ttg<1>() {
  Edge<int, int> I2N, N2N;
  Edge<void, int> N2S;

  auto init = make_tt<void>([](std::tuple<Out<int, int>> &outs) { send<0>(0, 0, outs); }, edges(), edges(I2N));

  auto next = make_tt<int>([](const int &key, int& value, std::tuple<Out<int, int>, Out<void, int>> &outs) {
    if (key < 10000000) {
      value++;
      send<0>(key+1, std::move(value), outs);
    }
    else {
      sendv<1>(std::move(value), outs);
    }
  } , edges(fuse(I2N, N2N)), edges(N2N, N2S));

  auto stop = make_tt<void>([](const int &v, std::tuple<> &outs) {
     std::cout << "last task received v=" << v << std::endl;
     ttg::default_execution_context().impl().final_task();
  }, edges(N2S), edges());

  return std::make_tuple(std::move(init), std::move(next), std::move(stop));
}

template <>
auto make_ttg<2>() {
  Edge<int, int> I2N1, I2N2;
  Edge<int, int> N2N1, N2N2;
  Edge<void, int> N2S1, N2S2;

  auto init = make_tt<void>([](std::tuple<Out<int, int>, Out<int, int>> &outs) {
    send<0>(0, 0, outs);
    send<1>(0, 0, outs);
  }, edges(), edges(I2N1, I2N2));

  auto next = make_tt<int>([](const int &key, int& v1, int& v2, std::tuple<Out<int, int>, Out<int, int>, Out<void, int>, Out<void, int>> &outs) {
    if (key < 10000000) {
      v1++; v2++;
      send<0>(key+1, std::move(v1), outs);
      send<1>(key+1, std::move(v2), outs);
    }
    else {
      sendv<2>(std::move(v1), outs);
      sendv<3>(std::move(v2), outs);
    }
  } , edges(fuse(I2N1, N2N1), fuse(I2N2, N2N2)), edges(N2N1, N2N2, N2S1, N2S2));

  auto stop = make_tt<void>([](const int &v1, const int &v2, std::tuple<> &outs) {
     std::cout << "last task received v=" << v1 << std::endl;
     ttg::default_execution_context().impl().final_task();
  }, edges(N2S1, N2S2), edges());

  return std::make_tuple(std::move(init), std::move(next), std::move(stop));
}

template <>
auto make_ttg<4>() {
  Edge<int, int> I2N1, I2N2, I2N3, I2N4;
  Edge<int, int> N2N1, N2N2, N2N3, N2N4;
  Edge<void, int> N2S1, N2S2, N2S3, N2S4;

  auto init = make_tt<void>([](std::tuple<Out<int, int>, Out<int, int>, Out<int, int>, Out<int, int>> &outs) {
    send<0>(0, 0, outs);
    send<1>(0, 0, outs);
    send<2>(0, 0, outs);
    send<3>(0, 0, outs);
  }, edges(), edges(I2N1, I2N2, I2N3, I2N4));

  auto next = make_tt<int>([](const int &key, int& v1, int& v2, int& v3, int& v4,
                              std::tuple<Out<int, int>, Out<int, int>,
                                         Out<int, int>, Out<int, int>,
                                         Out<void, int>, Out<void, int>,
                                         Out<void, int>, Out<void, int>> &outs) {
    if (key < 10000000) {
      v1++; v2++;
      v3++; v4++;
      send<0>(key+1, std::move(v1), outs);
      send<1>(key+1, std::move(v2), outs);
      send<2>(key+1, std::move(v3), outs);
      send<3>(key+1, std::move(v4), outs);
    }
    else {
      sendv<4>(std::move(v1), outs);
      sendv<5>(std::move(v2), outs);
      sendv<6>(std::move(v3), outs);
      sendv<7>(std::move(v4), outs);
    }
  }, edges(fuse(I2N1, N2N1), fuse(I2N2, N2N2),
           fuse(I2N3, N2N3), fuse(I2N4, N2N4)), 
     edges(N2N1, N2N2, N2N3, N2N4, N2S1, N2S2, N2S3, N2S4));

  auto stop = make_tt<void>([](const int &v1, const int &v2, const int &v3, const int &v4, std::tuple<> &outs) {
     std::cout << "last task received v=" << v1 << std::endl;
     ttg::default_execution_context().impl().final_task();
  }, edges(N2S1, N2S2, N2S3, N2S4), edges());

  return std::make_tuple(std::move(init), std::move(next), std::move(stop));
}

template <>
auto make_ttg<8>() {
  Edge<int, int> I2N1, I2N2, I2N3, I2N4, I2N5, I2N6, I2N7, I2N8;
  Edge<int, int> N2N1, N2N2, N2N3, N2N4, N2N5, N2N6, N2N7, N2N8;
  Edge<void, int> N2S1, N2S2, N2S3, N2S4, N2S5, N2S6, N2S7, N2S8;

  auto init = make_tt<void>([](std::tuple<Out<int, int>,
                                          Out<int, int>,
                                          Out<int, int>,
                                          Out<int, int>,
                                          Out<int, int>,
                                          Out<int, int>,
                                          Out<int, int>,
                                          Out<int, int>> &outs) {
    send<0>(0, 0, outs);
    send<1>(0, 0, outs);
    send<2>(0, 0, outs);
    send<3>(0, 0, outs);
    send<4>(0, 0, outs);
    send<5>(0, 0, outs);
    send<6>(0, 0, outs);
    send<7>(0, 0, outs);
  }, edges(), edges(I2N1, I2N2, I2N3, I2N4, I2N5, I2N6, I2N7, I2N8));

  auto next = make_tt<int>([](const int &key, int& v1, int& v2, int& v3, int& v4, int& v5, int& v6, int& v7, int& v8,
                              std::tuple<Out<int, int>, Out<int, int>, Out<int, int>, Out<int, int>, Out<int, int>, Out<int, int>, Out<int, int>, Out<int, int>,
                                         Out<void, int>, Out<void, int>, Out<void, int>, Out<void, int>, Out<void, int>, Out<void, int>, Out<void, int>, Out<void, int>> &outs) {
    if (key < 10000000) {
      v1++; v2++;
      v3++; v4++;
      v5++; v6++;
      v6++; v8++;
      send<0>(key+1, std::move(v1), outs);
      send<1>(key+1, std::move(v2), outs);
      send<2>(key+1, std::move(v3), outs);
      send<3>(key+1, std::move(v4), outs);
      send<4>(key+1, std::move(v5), outs);
      send<5>(key+1, std::move(v6), outs);
      send<6>(key+1, std::move(v7), outs);
      send<7>(key+1, std::move(v8), outs);
    }
    else {
      sendv<8>(std::move(v1), outs);
      sendv<9>(std::move(v2), outs);
      sendv<10>(std::move(v3), outs);
      sendv<11>(std::move(v4), outs);
      sendv<12>(std::move(v5), outs);
      sendv<13>(std::move(v6), outs);
      sendv<14>(std::move(v7), outs);
      sendv<15>(std::move(v8), outs);
    }
  }, edges(fuse(I2N1, N2N1), fuse(I2N2, N2N2), fuse(I2N3, N2N3), fuse(I2N4, N2N4), fuse(I2N5, N2N5), fuse(I2N6, N2N6), fuse(I2N7, N2N7), fuse(I2N8, N2N8)), 
     edges(N2N1, N2N2, N2N3, N2N4, N2N5, N2N6, N2N7, N2N8, N2S1, N2S2, N2S3, N2S4, N2S5, N2S6, N2S7, N2S8));

  auto stop = make_tt<void>([](const int &v1, const int &v2, const int &v3, const int &v4, const int &v5, const int &v6, const int &v7, const int &v8, std::tuple<> &outs) {
     std::cout << "last task received v=" << v1 << std::endl;
     ttg::default_execution_context().impl().final_task();
  }, edges(N2S1, N2S2, N2S3, N2S4, N2S5, N2S6, N2S7, N2S8), edges());

  return std::make_tuple(std::move(init), std::move(next), std::move(stop));
}

// flows task ids via keys
template <>
auto make_ttg<0>() {
  Edge<int, void> I2N, N2N;
  Edge<void, int> N2S;

  auto init = make_tt<void>([](std::tuple<Out<int, void>> &outs) { sendk<0>(0, outs); }, edges(), edges(I2N));

  auto next = make_tt([](const int& key, std::tuple<Out<int, void>, Out<void, int>> &outs) {
    if (key < 10000000) {
      ::sendk<0>(key+1, outs);
    }
    else {
      ::sendv<1>(key, outs);
    }
  }, edges(fuse(I2N, N2N)), edges(N2N, N2S));

  auto stop = make_tt<void>([](const int &v, std::tuple<> &outs) {
     std::cout << "last task received v=" << v << std::endl;
     ttg::default_execution_context().impl().final_task();
  }, edges(N2S), edges());

  return std::make_tuple(std::move(init), std::move(next), std::move(stop));
}

template<int num_flows>
void run_bench()
{
  auto [init, next, stop] = make_ttg<num_flows>();

  auto connected = make_graph_executable(init.get());
  assert(connected);
  std::cout << "Graph is connected.\n";

  auto t0 = now();
  if (ttg::default_execution_context().rank() == 0) init->invoke();

  ttg_execute(ttg_default_execution_context());
  ttg_fence(ttg_default_execution_context());
  auto t1 = now();

  std::cout << "# of tasks = " << N << std::endl;
  std::cout << "time elapsed (microseconds) = " << duration_in_mus(t0, t1) << std::endl;
}

int main(int argc, char* argv[]) {

  int num_flows = 0;
  ttg_initialize(argc, argv, -1);

  if (argc > 1) {
    num_flows = std::atoi(argv[1]);
  }

  switch(num_flows) {
  case 0: run_bench<0>(); break;
  case 1: run_bench<1>(); break;
  case 2: run_bench<2>(); break;
  case 4: run_bench<4>(); break;
  case 8: run_bench<8>(); break;
  default: std::cout << "Unsupported number of flows: " << num_flows << std::endl;
  }

  ttg_finalize();
  return 0;
}

