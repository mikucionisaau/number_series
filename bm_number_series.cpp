#include "number_series.h"
#include "number_series_wrap.h"
#include "number_series_wrap2.h"
#include "number_series_wrap3.h"

#include <benchmark/benchmark.h>

auto make_vv(size_t ns_number, size_t ns_length)
{
    using series = data_series::number_series;
    auto vv = std::vector<series>{};
    vv.reserve(ns_number);  // preallocate memory
    // populate vv with data here
    for (size_t i = 0; i < ns_number; ++i)
        vv.push_back(series::make_random(ns_length));
    for (auto& ns : vv)
        ns += series::make_random(ns_length);
    return vv;
};

auto make_wrapper(size_t ns_number, size_t ns_length)
{
    using series = data_series::number_series_wrap;
    auto vv = std::vector<series>{};
    vv.reserve(ns_number);  // preallocate memory
    // populate vv with data here
    for (size_t i = 0; i < ns_number; ++i)
        vv.push_back(series::make_random(ns_length));
    for (auto& ns : vv)
        ns += series::make_random(ns_length);
    return vv;
};

auto make_wrapper2(size_t ns_number, size_t ns_length)
{
    using series = data_series::number_series_wrap2;
    auto vv = std::vector<series>{};
    vv.reserve(ns_number);  // preallocate memory
    // populate vv with data here
    for (size_t i = 0; i < ns_number; ++i)
        vv.push_back(series::make_random(ns_length));
    for (auto& ns : vv)
        ns += series::make_random(ns_length);
    return vv;
};

auto make_wrapper3(size_t ns_number, size_t ns_length)
{
    using series = data_series::number_series_wrap3;
    auto vv = std::vector<series>{};
    vv.reserve(ns_number);  // preallocate memory
    // populate vv with data here
    for (size_t i = 0; i < ns_number; ++i)
        vv.push_back(series::make_random(ns_length));
    for (auto& ns : vv)
        ns += series::make_random(ns_length);
    return vv;
};

static void bm_ns_sort(benchmark::State& state)
{
    const auto ns_number = state.range(0);
    const auto ns_length = state.range(1);
    const auto input = make_vv(ns_number, ns_length);
    for (auto _ : state) {
        state.PauseTiming();
        auto vv = input;
        state.ResumeTiming();
        sort(vv.begin(), vv.end());
        benchmark::DoNotOptimize(vv.data());  // tells compiler that vv.data() is useful
        benchmark::ClobberMemory();           // flush changes to memory
    }
}
BENCHMARK(bm_ns_sort)->ArgPair(100'000, 100);

static void bm_ns_wrap_sort(benchmark::State& state)
{
    const auto ns_number = state.range(0);
    const auto ns_length = state.range(1);
    const auto input = make_wrapper(ns_number, ns_length);
    for (auto _ : state) {
        state.PauseTiming();
        auto vw = input;
        state.ResumeTiming();
        sort(vw.begin(), vw.end());
        benchmark::DoNotOptimize(vw.data());  // tells compiler that vw.data() is useful
        benchmark::ClobberMemory();           // flush changes to memory
    }
}
BENCHMARK(bm_ns_wrap_sort)->ArgPair(100'000, 100);

static void bm_ns_wrap2_sort(benchmark::State& state)
{
    const auto ns_number = state.range(0);
    const auto ns_length = state.range(1);
    const auto input = make_wrapper2(ns_number, ns_length);
    for (auto _ : state) {
        state.PauseTiming();
        auto vw = input;
        state.ResumeTiming();
        sort(vw.begin(), vw.end());
        benchmark::DoNotOptimize(vw.data());  // tells compiler that vw.data() is useful
        benchmark::ClobberMemory();           // flush changes to memory
    }
}
BENCHMARK(bm_ns_wrap2_sort)->ArgPair(100'000, 100);

static void bm_ns_wrap3_sort(benchmark::State& state)
{
    const auto ns_number = state.range(0);
    const auto ns_length = state.range(1);
    const auto input = make_wrapper3(ns_number, ns_length);
    for (auto _ : state) {
        state.PauseTiming();
        auto vw = input;
        state.ResumeTiming();
        sort(vw.begin(), vw.end());
        benchmark::DoNotOptimize(vw.data());  // tells compiler that vw.data() is useful
        benchmark::ClobberMemory();           // flush changes to memory
    }
}
BENCHMARK(bm_ns_wrap3_sort)->ArgPair(100'000, 100);

BENCHMARK_MAIN();

/*
=== Benchmarks with padding ===
Sample output comparison (bm_ns_vs_wrap) without padding:
https://github.com/mikucionisaau/number_series/actions/runs/4229495319/jobs/7345922364
GCC-12 on Ubuntu 22.04, 2 x 2793.44 MHz CPUs:
Benchmark                                        Time      CPU  Time Old  Time New  CPU Old  CPU New
----------------------------------------------------------------------------------------------------
[bm_ns_sort vs. bm_ns_wrap_sort]/100000/100   +1.1537  +1.1534        19        41       19       41
[bm_ns_sort vs. bm_ns_wrap2_sort]/100000/100  +0.6075  +0.6075        19        30       19       30
[bm_ns_sort vs. bm_ns_wrap3_sort]/100000/100  +0.6704  +0.6704        19        31       19       31

Interpretation: number_series 19, number_series_wrap 41ms
sort of number_series_wrap is 115% slower than number_series.

https://github.com/mikucionisaau/number_series/actions/runs/4229495319/jobs/7345922443
macOS, homebrew GCC-12 on 3 x 3337 MHz CPUs:
Benchmark                                        Time      CPU  Time Old  Time New  CPU Old  CPU New
----------------------------------------------------------------------------------------------------
[bm_ns_sort vs. bm_ns_wrap_sort]/100000/100   +0.5463  +0.6560        81       125       75      123
[bm_ns_sort vs. bm_ns_wrap2_sort]/100000/100  +0.1314  +0.1185       108       122       90      101
[bm_ns_sort vs. bm_ns_wrap3_sort]/100000/100  +0.0878  +0.1435       101       110       83       94

https://github.com/mikucionisaau/number_series/actions/runs/4229495319/jobs/7345922513
Windows, MSVC on 2 x 2818.9 MHz CPUs:
Benchmark                                        Time      CPU  Time Old  Time New  CPU Old  CPU New
----------------------------------------------------------------------------------------------------
[bm_ns_sort vs. bm_ns_wrap_sort]/100000/100   +2.3589  +2.3793        26        86       26       86
[bm_ns_sort vs. bm_ns_wrap2_sort]/100000/100  +0.5438  +0.5722        26        40       26       40
[bm_ns_sort vs. bm_ns_wrap3_sort]/100000/100  +0.6570  +0.6758        26        43       25       43

=== Benchmarks with padding ===
Sample output comparison (bm_ns_vs_wrap) with padding of 100 integers:
https://github.com/mikucionisaau/number_series/actions/runs/4222606513/jobs/7331312313
GCC-12 on Ubuntu 22.04, 2 x 2593.91 MHz CPUs:
Benchmark                                        Time      CPU  Time Old  Time New  CPU Old  CPU New
----------------------------------------------------------------------------------------------------
[bm_ns_sort vs. bm_ns_wrap_sort]/100000/100   -0.0708  -0.0707        64        59       64       59
[bm_ns_sort vs. bm_ns_wrap2_sort]/100000/100  -0.2489  -0.2489        63        47       63       47
[bm_ns_sort vs. bm_ns_wrap3_sort]/100000/100  -0.1719  -0.1719        63        52       63       52

Interpretation:
number_series_wrap, number_series_wrap2 and number_series_wrap3 are faster than plain number_series.
number_series_wrap2 has the best performance (faster than number_series by 24.89%).

https://github.com/mikucionisaau/number_series/actions/runs/4222606513/jobs/7331312447
macOS, Homebrew GCC-12 on 3 x 2574 MHz CPUs:
Benchmark                                        Time      CPU  Time Old  Time New  CPU Old  CPU New
----------------------------------------------------------------------------------------------------
[bm_ns_sort vs. bm_ns_wrap_sort]/100000/100   +0.8013  +0.8019       122       220      122      220
[bm_ns_sort vs. bm_ns_wrap2_sort]/100000/100  +0.3323  +0.3326       120       160      120      160
[bm_ns_sort vs. bm_ns_wrap3_sort]/100000/100  +0.3619  +0.3621       119       162      119      162

https://github.com/mikucionisaau/number_series/actions/runs/4222606513/jobs/7331312658
Windows, MSVC on 2 x 2627.32 MHz CPUs:
Benchmark                                        Time      CPU  Time Old  Time New  CPU Old  CPU New
----------------------------------------------------------------------------------------------------
[bm_ns_sort vs. bm_ns_wrap_sort]/100000/100   +0.0169  +0.0192       127       129      127      130
[bm_ns_sort vs. bm_ns_wrap2_sort]/100000/100  -0.4605  -0.4610       130        70      130       70
[bm_ns_sort vs. bm_ns_wrap3_sort]/100000/100  -0.4364  -0.4404       134        76      135       75

 */