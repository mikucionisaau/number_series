// Created by Marius Mikucionis <marius@cs.aau.dk>

#include "number_series.hpp"
#include <benchmark/benchmark.h>

/// Benchmark all number_series::make_random implementations

using namespace data_series;

static void bm_make_random1(benchmark::State& state)
{
    const auto count = state.range(0);
    for (auto _ : state) {
        auto ns = number_series::make_random(count);
        benchmark::DoNotOptimize(ns.data());  // tells compiler that ns.data() is useful
        benchmark::ClobberMemory();           // flush changes to memory
    }
}
BENCHMARK(bm_make_random1)->Arg(100)->Arg(1'000)->Arg(10'000)->Arg(100'000);

static void bm_make_random2(benchmark::State& state)
{
    const auto count = state.range(0);
    for (auto _ : state) {
        auto ns = number_series::make_random2(count);
        benchmark::DoNotOptimize(ns.data());  // tells compiler that ns.data() is useful
        benchmark::ClobberMemory();           // flush changes to memory
    }
}
BENCHMARK(bm_make_random2)->Arg(100)->Arg(1'000)->Arg(10'000)->Arg(100'000);

static void bm_make_random3(benchmark::State& state)
{
    const auto count = state.range(0);
    for (auto _ : state) {
        auto ns = number_series::make_random3(count);
        benchmark::DoNotOptimize(ns.data());  // tells compiler that ns.data() is useful
        benchmark::ClobberMemory();           // flush changes to memory
    }
}
BENCHMARK(bm_make_random3)->Arg(100)->Arg(1'000)->Arg(10'000)->Arg(100'000);

static void bm_make_random4(benchmark::State& state)
{
    const auto count = state.range(0);
    for (auto _ : state) {
        auto ns = number_series::make_random4(count);
        benchmark::DoNotOptimize(ns.data());  // tells compiler that ns.data() is useful
        benchmark::ClobberMemory();           // flush changes to memory
    }
}
BENCHMARK(bm_make_random4)->Arg(100)->Arg(1'000)->Arg(10'000)->Arg(100'000);

static void bm_make_random5(benchmark::State& state)
{
    const auto count = state.range(0);
    for (auto _ : state) {
        auto ns = number_series::make_random5(count);
        benchmark::DoNotOptimize(ns.data());  // tells compiler that ns.data() is useful
        benchmark::ClobberMemory();           // flush changes to memory
    }
}
BENCHMARK(bm_make_random5)->Arg(100)->Arg(1'000)->Arg(10'000)->Arg(100'000);

BENCHMARK_MAIN();
