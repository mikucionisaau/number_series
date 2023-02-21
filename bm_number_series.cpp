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

Sample output comparison (bm_ns_vs_wrap) without padding:
Benchmark                                       Time      CPU  Time Old  Time New   CPU Old   CPU New
-----------------------------------------------------------------------------------------------------
[bm_ns_sort vs. bm_ns_wrap_sort]/100000/100  +1.7062  +1.7064  18402147  49799978  18400028  49797757
OVERALL_GEOMEAN                              +1.7062  +1.7064         0         0         0         0

Interpretation: number_series 18.402147ms, number_series_wrap 49.799978ms
sort of number_series_wrap is 170% slower than number_series.

Sample output comparison (bm_ns_vs_wrap) with padding of 100 integers:
GCC-12 on Ubuntu 22.04, 2 x 2593.91 MHz CPUs
Benchmark                                        Time      CPU  Time Old  Time New  CPU Old  CPU New
----------------------------------------------------------------------------------------------------
[bm_ns_sort vs. bm_ns_wrap_sort]/100000/100   -0.0708  -0.0707        64        59       64       59
OVERALL_GEOMEAN                               -0.0708  -0.0707         0         0        0        0

Benchmark                                        Time      CPU  Time Old  Time New  CPU Old  CPU New
----------------------------------------------------------------------------------------------------
[bm_ns_sort vs. bm_ns_wrap2_sort]/100000/100  -0.2489  -0.2489        63        47       63       47
OVERALL_GEOMEAN                               -0.2489  -0.2489         0         0        0        0

Benchmark                                        Time      CPU  Time Old  Time New  CPU Old  CPU New
----------------------------------------------------------------------------------------------------
[bm_ns_sort vs. bm_ns_wrap3_sort]/100000/100  -0.1719  -0.1719        63        52       63       52
OVERALL_GEOMEAN                               -0.1719  -0.1719         0         0        0        0

Interpretation:
number_series_wrap, number_series_wrap2 and number_series_wrap3 are faster than plain number_series.
number_series_wrap2 has the best performance (faster than number_series by 24.89%).

macOS, Homebrew GCC-12 on 3 x 2574 MHz CPUs
Benchmark                                        Time      CPU  Time Old  Time New  CPU Old  CPU New
----------------------------------------------------------------------------------------------------
[bm_ns_sort vs. bm_ns_wrap_sort]/100000/100   +0.8013  +0.8019       122       220      122      220
OVERALL_GEOMEAN                               +0.8013  +0.8019         0         0        0        0

Benchmark                                        Time      CPU  Time Old  Time New  CPU Old  CPU New
----------------------------------------------------------------------------------------------------
[bm_ns_sort vs. bm_ns_wrap2_sort]/100000/100  +0.3323  +0.3326       120       160      120      160
OVERALL_GEOMEAN                               +0.3323  +0.3326         0         0        0        0

Benchmark                                        Time      CPU  Time Old  Time New  CPU Old  CPU New
----------------------------------------------------------------------------------------------------
[bm_ns_sort vs. bm_ns_wrap3_sort]/100000/100  +0.3619  +0.3621       119       162      119      162
OVERALL_GEOMEAN                               +0.3619  +0.3621         0         0        0        0

Windows, MSVC on 2 x 2627.32 MHz CPUs
Benchmark                                        Time      CPU  Time Old  Time New  CPU Old  CPU New
----------------------------------------------------------------------------------------------------
[bm_ns_sort vs. bm_ns_wrap_sort]/100000/100   +0.0169  +0.0192       127       129      127      130
OVERALL_GEOMEAN                               +0.0169  +0.0192         0         0        0        0

Benchmark                                        Time      CPU  Time Old  Time New  CPU Old  CPU New
----------------------------------------------------------------------------------------------------
[bm_ns_sort vs. bm_ns_wrap2_sort]/100000/100  -0.4605  -0.4610       130        70      130       70
OVERALL_GEOMEAN                               -0.4605  -0.4610         0         0        0        0

Benchmark                                        Time      CPU  Time Old  Time New  CPU Old  CPU New
----------------------------------------------------------------------------------------------------
[bm_ns_sort vs. bm_ns_wrap3_sort]/100000/100  -0.4364  -0.4404       134        76      135       75
OVERALL_GEOMEAN                               -0.4364  -0.4404         0         0        0        0

 */