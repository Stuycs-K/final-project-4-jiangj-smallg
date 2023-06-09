#include <iostream>
#include <random>
#include <benchmark/benchmark.h>

#include "Hash.hpp"
#include "hash/SHA.hpp"

#include "ProfileUtil.hpp"

#define REPEAT2(x) x x
#define REPEAT4(x) REPEAT2(x) REPEAT2(x)
#define REPEAT8(x) REPEAT4(x) REPEAT4(x)
#define REPEAT16(x) REPEAT8(x) REPEAT8(x)
#define REPEAT32(x) REPEAT16(x) REPEAT16(x)
#define REPEAT64(x) REPEAT32(x) REPEAT32(x)

constexpr std::size_t TARGET_BYTES = 1 << 24;
using rand_t = decltype(rand());

void BM_SHA256(benchmark::State& state)
{
    srand(1);
    const unsigned int N = state.range(0);
    const unsigned int total_elements = N / sizeof(rand_t);

    rand_t* rand_buffer = new rand_t[total_elements];
    char* msg = new char[total_elements * sizeof(rand_t)];
    
    for (std::size_t i = 0; i < total_elements; ++i) rand_buffer[i] = rand();
    std::memcpy(msg, rand_buffer, total_elements * sizeof(rand_t));

    Hash<256> hash;
    for (auto _ : state)
    {
        REPEAT64(hash = sha256(msg);)   
        benchmark::DoNotOptimize(hash);
        benchmark::ClobberMemory();
    }

    delete[] rand_buffer;
    delete[] msg;

    state.SetItemsProcessed(state.iterations() * 64);
    state.SetBytesProcessed(state.iterations() * 64 * total_elements * sizeof(rand_t));
}

void BM_NONVECTORIZED_SHA256(benchmark::State& state)
{
    srand(1);
    const unsigned int N = state.range(0);
    const unsigned int total_elements = N / sizeof(rand_t);

    rand_t* rand_buffer = new rand_t[total_elements];
    char* msg = new char[total_elements * sizeof(rand_t)];
    
    for (std::size_t i = 0; i < total_elements; ++i) rand_buffer[i] = rand();
    std::memcpy(msg, rand_buffer, total_elements * sizeof(rand_t));

    Hash<256> hash;
    for (auto _ : state)
    {
        REPEAT64(hash = _details::general_sha256(msg);)   
        benchmark::DoNotOptimize(hash);
        benchmark::ClobberMemory();
    }

    delete[] rand_buffer;
    delete[] msg;

    state.SetItemsProcessed(state.iterations() * 64);
    state.SetBytesProcessed(state.iterations() * 64 * total_elements * sizeof(rand_t));
}

void BM_NONVECTORIZED_SHA224(benchmark::State& state)
{
    srand(1);
    const unsigned int N = state.range(0);
    const unsigned int total_elements = N / sizeof(rand_t);

    rand_t* rand_buffer = new rand_t[total_elements];
    char* msg = new char[total_elements * sizeof(rand_t)];
    
    for (std::size_t i = 0; i < total_elements; ++i) rand_buffer[i] = rand();
    std::memcpy(msg, rand_buffer, total_elements * sizeof(rand_t));

    Hash<224> hash;
    for (auto _ : state)
    {
        REPEAT64(hash = _details::general_sha224(msg);)   
        benchmark::DoNotOptimize(hash);
        benchmark::ClobberMemory();
    }

    delete[] rand_buffer;
    delete[] msg;

    state.SetItemsProcessed(state.iterations() * 64);
    state.SetBytesProcessed(state.iterations() * 64 * total_elements * sizeof(rand_t));
}

#ifdef __AVX2__
void BM_VECTORIZED_SHA256(benchmark::State& state)
{
    srand(1);
    const unsigned int N = state.range(0);
    const unsigned int total_elements = N / sizeof(rand_t);

    rand_t* rand_buffer = new rand_t[total_elements];
    char* msg = new char[total_elements * sizeof(rand_t)];
    
    for (std::size_t i = 0; i < total_elements; ++i) rand_buffer[i] = rand();
    std::memcpy(msg, rand_buffer, total_elements * sizeof(rand_t));

    Hash<256> hash;
    for (auto _ : state)
    {
        REPEAT64(hash = _details::simd_sha256(msg);)   
        benchmark::DoNotOptimize(hash);
        benchmark::ClobberMemory();
    }

    delete[] rand_buffer;
    delete[] msg;

    state.SetItemsProcessed(state.iterations() * 64);
    state.SetBytesProcessed(state.iterations() * 64 * total_elements * sizeof(rand_t));
}


void BM_VECTORIZED_SHA224(benchmark::State& state)
{
    srand(1);
    const unsigned int N = state.range(0);
    const unsigned int total_elements = N / sizeof(rand_t);

    rand_t* rand_buffer = new rand_t[total_elements];
    char* msg = new char[total_elements * sizeof(rand_t)];
    
    for (std::size_t i = 0; i < total_elements; ++i) rand_buffer[i] = rand();
    std::memcpy(msg, rand_buffer, total_elements * sizeof(rand_t));

    Hash<224> hash;
    for (auto _ : state)
    {
        REPEAT64(hash = _details::simd_sha224(msg);)   
        benchmark::DoNotOptimize(hash);
        benchmark::ClobberMemory();
    }

    delete[] rand_buffer;
    delete[] msg;

    state.SetItemsProcessed(state.iterations() * 64);
    state.SetBytesProcessed(state.iterations() * 64 * total_elements * sizeof(rand_t));
}

#endif

#ifdef __SHA__
void BM_INTRINSIC_SHA256(benchmark::State& state)
{
    srand(1);
    const unsigned int N = state.range(0);
    const unsigned int total_elements = N / sizeof(rand_t);

    rand_t* rand_buffer = new rand_t[total_elements];
    char* msg = new char[total_elements * sizeof(rand_t)];
    
    for (std::size_t i = 0; i < total_elements; ++i) rand_buffer[i] = rand();
    std::memcpy(msg, rand_buffer, total_elements * sizeof(rand_t));

    Hash<256> hash;
    for (auto _ : state)
    {
        REPEAT64(hash = _details::instruction_sha256(msg);)   
        benchmark::DoNotOptimize(hash);
        benchmark::ClobberMemory();
    }

    delete[] rand_buffer;
    delete[] msg;

    state.SetItemsProcessed(state.iterations() * 64);
    state.SetBytesProcessed(state.iterations() * 64 * total_elements * sizeof(rand_t));
}

void BM_INTRINSIC_SHA224(benchmark::State& state)
{
    srand(1);
    const unsigned int N = state.range(0);
    const unsigned int total_elements = N / sizeof(rand_t);

    rand_t* rand_buffer = new rand_t[total_elements];
    char* msg = new char[total_elements * sizeof(rand_t)];
    
    for (std::size_t i = 0; i < total_elements; ++i) rand_buffer[i] = rand();
    std::memcpy(msg, rand_buffer, total_elements * sizeof(rand_t));

    Hash<224> hash;
    for (auto _ : state)
    {
        REPEAT64(hash = _details::instruction_sha224(msg);)   
        benchmark::DoNotOptimize(hash);
        benchmark::ClobberMemory();
    }

    delete[] rand_buffer;
    delete[] msg;

    state.SetItemsProcessed(state.iterations() * 64);
    state.SetBytesProcessed(state.iterations() * 64 * total_elements * sizeof(rand_t));
}
#endif

void BM_SHA224(benchmark::State& state)
{
    srand(1);
    const unsigned int N = state.range(0);
    const unsigned int total_elements = N / sizeof(rand_t);

    rand_t* rand_buffer = new rand_t[total_elements];
    char* msg = new char[total_elements * sizeof(rand_t)];
    
    for (std::size_t i = 0; i < total_elements; ++i) rand_buffer[i] = rand();
    std::memcpy(msg, rand_buffer, total_elements * sizeof(rand_t));

    Hash<224> hash;
    for (auto _ : state)
    {
        REPEAT64(hash = sha224(msg);)   
        benchmark::DoNotOptimize(hash);
        benchmark::ClobberMemory();
    }

    delete[] rand_buffer;
    delete[] msg;

    state.SetItemsProcessed(state.iterations() * 64);
    state.SetBytesProcessed(state.iterations() * 64 * total_elements * sizeof(rand_t));
}

void BM_NONVECTORIZED_SHA512(benchmark::State& state)
{
    srand(1);
    const unsigned int N = state.range(0);
    const unsigned int total_elements = N / sizeof(rand_t);

    rand_t* rand_buffer = new rand_t[total_elements];
    char* msg = new char[total_elements * sizeof(rand_t)];
    
    for (std::size_t i = 0; i < total_elements; ++i) rand_buffer[i] = rand();
    std::memcpy(msg, rand_buffer, total_elements * sizeof(rand_t));

    Hash<512> hash;
    for (auto _ : state)
    {
        REPEAT64(hash = _details::general_sha512(msg);)   
        benchmark::DoNotOptimize(hash);
        benchmark::ClobberMemory();
    }

    delete[] rand_buffer;
    delete[] msg;

    state.SetItemsProcessed(state.iterations() * 64);
    state.SetBytesProcessed(state.iterations() * 64 * total_elements * sizeof(rand_t));
}

void BM_NONVECTORIZED_SHA384(benchmark::State& state)
{
    srand(1);
    const unsigned int N = state.range(0);
    const unsigned int total_elements = N / sizeof(rand_t);

    rand_t* rand_buffer = new rand_t[total_elements];
    char* msg = new char[total_elements * sizeof(rand_t)];
    
    for (std::size_t i = 0; i < total_elements; ++i) rand_buffer[i] = rand();
    std::memcpy(msg, rand_buffer, total_elements * sizeof(rand_t));

    Hash<384> hash;
    for (auto _ : state)
    {
        REPEAT64(hash = _details::general_sha384(msg);)   
        benchmark::DoNotOptimize(hash);
        benchmark::ClobberMemory();
    }

    delete[] rand_buffer;
    delete[] msg;

    state.SetItemsProcessed(state.iterations() * 64);
    state.SetBytesProcessed(state.iterations() * 64 * total_elements * sizeof(rand_t));
}

// BENCHMARK(BM_SHA256)->Arg(TARGET_BYTES);
BENCHMARK(BM_NONVECTORIZED_SHA256)->Arg(TARGET_BYTES);
#ifdef __AVX2__
BENCHMARK(BM_VECTORIZED_SHA256)->Arg(TARGET_BYTES);
#endif
#ifdef __SHA__
BENCHMARK(BM_INTRINSIC_SHA256)->Arg(TARGET_BYTES);
#endif

// BENCHMARK(BM_SHA224)->Arg(TARGET_BYTES);
BENCHMARK(BM_NONVECTORIZED_SHA224)->Arg(TARGET_BYTES);
#ifdef __AVX2__
BENCHMARK(BM_VECTORIZED_SHA224)->Arg(TARGET_BYTES);
#endif
#ifdef __SHA__
BENCHMARK(BM_INTRINSIC_SHA224)->Arg(TARGET_BYTES);
#endif

BENCHMARK(BM_NONVECTORIZED_SHA512)->Arg(TARGET_BYTES);

BENCHMARK(BM_NONVECTORIZED_SHA384)->Arg(TARGET_BYTES);

BENCHMARK_MAIN();