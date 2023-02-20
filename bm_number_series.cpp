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

Sample output comparison (bm_ns_vs_wrap) with padding:
Benchmark                                       Time      CPU  Time Old  Time New   CPU Old   CPU New
-----------------------------------------------------------------------------------------------------
[bm_ns_sort vs. bm_ns_wrap_sort]/100000/100  -0.2084  -0.2084  74110795  58663856  74106409  58661904
OVERALL_GEOMEAN                              -0.2084  -0.2084         0         0         0         0

Interpretation: number_series 74.110795ms, number_series_wrap 58.663856ms
sort of number_series_wrap is 20.8% faster than padded number_series.

 */