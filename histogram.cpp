/*
 * Copyright (c) 2024 Romain BAILLY
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <cassert>
#include <chrono>
#include <cstdint>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <emmintrin.h>
#include <random>


void histo_naive(uint32_t histo[], const uint8_t data[], size_t size) noexcept
{
    memset(histo, 0, 256*sizeof(histo[0]));
    for (size_t i=0; i<size; ++i)
        histo[data[i]]++;
}


void histo_read32(uint32_t histo[], const uint8_t data[], size_t size) noexcept
{
    memset(histo, 0, 256*sizeof(histo[0]));

    const uint32_t* data32 = reinterpret_cast<const uint32_t*>(data);
    for (size_t i=0; i<size/4; ++i)
    {
        uint32_t w = data32[i];
        histo[ w      & 0xFF]++;
        histo[(w>>8)  & 0xFF]++;
        histo[(w>>16) & 0xFF]++;
        histo[ w>>24]++;
    }
}


void histo_read64(uint32_t histo[], const uint8_t data[], size_t size) noexcept
{
    memset(histo, 0, 256*sizeof(histo[0]));

    const uint64_t* data64 = reinterpret_cast<const uint64_t*>(data);
    for (size_t i=0; i<size/8; ++i)
    {
        uint64_t w = data64[i];
        histo[ w      & 0xFF]++;
        histo[(w>>8)  & 0xFF]++;
        histo[(w>>16) & 0xFF]++;
        histo[(w>>24) & 0xFF]++;
        histo[(w>>32) & 0xFF]++;
        histo[(w>>40) & 0xFF]++;
        histo[(w>>48) & 0xFF]++;
        histo[ w>>56]++;
    }
}


void histo_read32_tmp2(uint32_t histo[], const uint8_t data[], size_t size) noexcept
{
    uint32_t tmp[2][256];
    memset(tmp, 0, sizeof(tmp));

    const uint32_t* data32 = reinterpret_cast<const uint32_t*>(data);
    for (size_t i=0; i<size/4; ++i)
    {
        uint32_t w = data32[i];
        tmp[0][ w      & 0xFF]++;
        tmp[1][(w>>8)  & 0xFF]++;
        tmp[0][(w>>16) & 0xFF]++;
        tmp[1][ w>>24]++;
    }

    for (size_t i=0; i<256u; ++i)
        histo[i] = tmp[0][i] + tmp[1][i];
}


template<unsigned N>
void sum_histo(uint32_t histo[256], const uint32_t tmp[N][256]) noexcept
{
    for (size_t i=0; i<256u; ++i)
    {
        uint32_t h = tmp[0][i];
        for (size_t j=1; j<N; ++j)
            h += tmp[j][i];
        histo[i] = h;
    }
}


void sum_histo8_simd(uint32_t histo[256], const uint32_t tmp[8][256]) noexcept
{
    __m128i* s = reinterpret_cast<__m128i*>(histo);
    for (size_t b=0; b<256u; b+=16)
    {
        __m128i s0 = *reinterpret_cast<const __m128i*>(tmp[0] + b);
        __m128i s1 = *reinterpret_cast<const __m128i*>(tmp[0] + b+4);
        __m128i s2 = *reinterpret_cast<const __m128i*>(tmp[0] + b+8);
        __m128i s3 = *reinterpret_cast<const __m128i*>(tmp[0] + b+12);
        s0 = _mm_add_epi32(s0, *reinterpret_cast<const __m128i*>(tmp[1] + b));
        s1 = _mm_add_epi32(s1, *reinterpret_cast<const __m128i*>(tmp[1] + b+4));
        s2 = _mm_add_epi32(s2, *reinterpret_cast<const __m128i*>(tmp[1] + b+8));
        s3 = _mm_add_epi32(s3, *reinterpret_cast<const __m128i*>(tmp[1] + b+12));
        s0 = _mm_add_epi32(s0, *reinterpret_cast<const __m128i*>(tmp[2] + b));
        s1 = _mm_add_epi32(s1, *reinterpret_cast<const __m128i*>(tmp[2] + b+4));
        s2 = _mm_add_epi32(s2, *reinterpret_cast<const __m128i*>(tmp[2] + b+8));
        s3 = _mm_add_epi32(s3, *reinterpret_cast<const __m128i*>(tmp[2] + b+12));
        s0 = _mm_add_epi32(s0, *reinterpret_cast<const __m128i*>(tmp[3] + b));
        s1 = _mm_add_epi32(s1, *reinterpret_cast<const __m128i*>(tmp[3] + b+4));
        s2 = _mm_add_epi32(s2, *reinterpret_cast<const __m128i*>(tmp[3] + b+8));
        s3 = _mm_add_epi32(s3, *reinterpret_cast<const __m128i*>(tmp[3] + b+12));
        s0 = _mm_add_epi32(s0, *reinterpret_cast<const __m128i*>(tmp[4] + b));
        s1 = _mm_add_epi32(s1, *reinterpret_cast<const __m128i*>(tmp[4] + b+4));
        s2 = _mm_add_epi32(s2, *reinterpret_cast<const __m128i*>(tmp[4] + b+8));
        s3 = _mm_add_epi32(s3, *reinterpret_cast<const __m128i*>(tmp[4] + b+12));
        s0 = _mm_add_epi32(s0, *reinterpret_cast<const __m128i*>(tmp[5] + b));
        s1 = _mm_add_epi32(s1, *reinterpret_cast<const __m128i*>(tmp[5] + b+4));
        s2 = _mm_add_epi32(s2, *reinterpret_cast<const __m128i*>(tmp[5] + b+8));
        s3 = _mm_add_epi32(s3, *reinterpret_cast<const __m128i*>(tmp[5] + b+12));
        s0 = _mm_add_epi32(s0, *reinterpret_cast<const __m128i*>(tmp[6] + b));
        s1 = _mm_add_epi32(s1, *reinterpret_cast<const __m128i*>(tmp[6] + b+4));
        s2 = _mm_add_epi32(s2, *reinterpret_cast<const __m128i*>(tmp[6] + b+8));
        s3 = _mm_add_epi32(s3, *reinterpret_cast<const __m128i*>(tmp[6] + b+12));
        s0 = _mm_add_epi32(s0, *reinterpret_cast<const __m128i*>(tmp[7] + b));
        s1 = _mm_add_epi32(s1, *reinterpret_cast<const __m128i*>(tmp[7] + b+4));
        s2 = _mm_add_epi32(s2, *reinterpret_cast<const __m128i*>(tmp[7] + b+8));
        s3 = _mm_add_epi32(s3, *reinterpret_cast<const __m128i*>(tmp[7] + b+12));
        *s++ = s0;
        *s++ = s1;
        *s++ = s2;
        *s++ = s3;
   }
}


template<unsigned N, typename Word>
void histo_tmp(uint32_t histo[], const uint8_t data[], size_t size) noexcept
{
    static_assert(!(N & (N-1)) && 2<=N && N<=sizeof(Word), "N must be a power of 2");

    uint32_t tmp[N][256];
    memset(tmp, 0, sizeof(tmp));

    const Word* ptr = reinterpret_cast<const Word*>(data);
    for (size_t i=0; i<size / sizeof(Word); ++i)
    {
#if (defined(_M_IX86) || defined(_M_X64))
        #define update(h, n)  h[uint8_t(w)]++; w>>=8;
#else
        #define update(h, n)  h[(w >> ((n)*8)) & 0xFF]++;
#endif

        Word w = ptr[i];
        update(tmp[0%N], 0)
        update(tmp[1%N], 1)
        update(tmp[2%N], 2)
        if constexpr(sizeof(Word) >= 8u)
        {
            update(tmp[3%N], 3)
            update(tmp[4%N], 4)
            update(tmp[5%N], 5)
            update(tmp[6%N], 6)
        }
        update(tmp[N-1], sizeof(Word)-1)

        #undef update
    }

    sum_histo<N>(histo, tmp);
}


#define histo_read32_tmp4  histo_tmp<4,uint32_t>
#define histo_read64_tmp4  histo_tmp<4,uint64_t>
#define histo_read64_tmp8  histo_tmp<8,uint64_t>


template<unsigned N, typename Word, void(*sum_histo)(uint32_t*,const uint32_t[][256])noexcept>
void histo_tmp_unrolled4(uint32_t histo[], const uint8_t data[], size_t size) noexcept
{
    static_assert(!(N & (N-1)) && 2<=N && N<=16, "N must be a power of 2");

    uint32_t tmp[N][256];
    memset(tmp, 0, sizeof(tmp));

    const size_t bytesPerLoop = sizeof(Word) * 4;
    const size_t loopCount    = size / bytesPerLoop;

    const Word* ptr = reinterpret_cast<const Word*>(data);
    size_t i = loopCount;
    do
    {
        Word a = *ptr++;
        Word b = *ptr++;
        Word c = *ptr++;
        Word d = *ptr++;

#if (defined(_M_IX86) || defined(_M_X64))
        #define update(h, w, n)    h[uint8_t(w)]++;  w>>=8;
#else
        #define update_histo(h, w, n)    h[(w >> ((n)*8)) & 0xFF]++;
#endif

        update(tmp[ 0%N], a, 0)
        update(tmp[ 1%N], b, 0)
        update(tmp[ 2%N], c, 0)
        update(tmp[ 3%N], d, 0)
        update(tmp[ 4%N], a, 1)
        update(tmp[ 5%N], b, 1)
        update(tmp[ 6%N], c, 1)
        update(tmp[ 7%N], d, 1)
        update(tmp[ 8%N], a, 2)
        update(tmp[ 9%N], b, 2)
        update(tmp[10%N], c, 2)
        update(tmp[11%N], d, 2)
        if constexpr(sizeof(Word) >= 8)
        {
            update(tmp[12%N], a, 3)
            update(tmp[13%N], b, 3)
            update(tmp[14%N], c, 3)
            update(tmp[15%N], d, 3)
            update(tmp[ 0%N], a, 4)
            update(tmp[ 1%N], b, 4)
            update(tmp[ 2%N], c, 4)
            update(tmp[ 3%N], d, 4)
            update(tmp[ 4%N], a, 5)
            update(tmp[ 5%N], b, 5)
            update(tmp[ 6%N], c, 5)
            update(tmp[ 7%N], d, 5)
            update(tmp[ 8%N], a, 6)
            update(tmp[ 9%N], b, 6)
            update(tmp[10%N], c, 6)
            update(tmp[11%N], d, 6)
        }
        update(tmp[12%N], a, sizeof(Word)-1)
        update(tmp[13%N], b, sizeof(Word)-1)
        update(tmp[14%N], c, sizeof(Word)-1)
        update(tmp[15%N], d, sizeof(Word)-1)

        #undef update
    }
    while (--i);

    sum_histo(histo, tmp);
}


#define histo_read64_tmp4_unrolled4  histo_tmp_unrolled4<4, uint64_t, sum_histo<4>>
#define histo_read64_tmp8_unrolled4  histo_tmp_unrolled4<8, uint64_t, sum_histo<8>>
#define histo_read64_tmp8_unrolled4_simd  histo_tmp_unrolled4<8, uint64_t, sum_histo8_simd>


#if USE_ASM

#if _MSC_VER
    #define ASM_ABI
#else
    #define ASM_ABI  __attribute__((ms_abi))
#endif

extern "C" void ASM_ABI rmgr_histo_x86(size_t loopCount, const void* data, uint32_t histo[8][256]) noexcept;
extern "C" void ASM_ABI rmgr_histo_x86_bmi(size_t loopCount, const void* data, uint32_t histo[8][256]) noexcept;


void histo_asm(uint32_t histo[], const uint8_t data[], size_t size) noexcept
{
    alignas(64) uint32_t tmp[8][256];
    memset(tmp, 0, sizeof(tmp));

    constexpr size_t bytesPerLoop = 32;
    const     size_t loopCount    = size / bytesPerLoop;
    rmgr_histo_x86(loopCount, data, tmp);
    sum_histo8_simd(histo, tmp);
}


void histo_asm_bmi(uint32_t histo[], const uint8_t data[], size_t size) noexcept
{
    alignas(64) uint32_t tmp[8][256];
    memset(tmp, 0, sizeof(tmp));

    constexpr size_t bytesPerLoop = 32;
    const     size_t loopCount    = size / bytesPerLoop;
    rmgr_histo_x86_bmi(loopCount, data, tmp);
    sum_histo8_simd(histo, tmp);
}


#endif


void bench_histo(void (*fct)(uint32_t*, const uint8_t*, size_t) noexcept, const char* name, const uint8_t data[], size_t size, unsigned rounds) noexcept
{
    uint32_t histo[256];

#ifndef NDEBUG
    if (fct != histo_naive)
    {
        uint32_t histo2[256];
        fct(histo, data, size);
        histo_naive(histo2, data, size);
        assert(memcmp(histo, histo2, 256*sizeof(histo[0])) == 0);
    }
#endif

    using Clock    = std::chrono::high_resolution_clock;
    using Duration = Clock::duration;
    Duration t = Clock::duration::max();
    for (unsigned i=0; i<rounds; ++i)
    {
        const auto t1 = Clock::now();
        fct(histo, data, size);
        const auto t2 = Clock::now();

        const Duration newT = t2 - t1;
        if (newT < t)
        {
            t = newT;
            double seconds = t.count() * (double(Duration::period::num) / double(Duration::period::den));
            double mibs    = size / (1024*1024.0);
            printf("\r%-30s %4.0f MiB/s", name, mibs/seconds);
        }
    }
    printf("\n");
}


void bench_all(const uint8_t* data, size_t size) noexcept
{
    const unsigned rounds = 20;

    #define BENCH(x) bench_histo(histo_##x, #x, data, size, rounds)
    BENCH(naive);
    BENCH(read32);
    BENCH(read64);
    BENCH(read32_tmp2);
    BENCH(read32_tmp4);
    BENCH(read64_tmp4);
    BENCH(read64_tmp8);
    BENCH(read64_tmp4_unrolled4);
    BENCH(read64_tmp8_unrolled4);
    BENCH(read64_tmp8_unrolled4_simd);
#if USE_ASM
    BENCH(asm);
    BENCH(asm_bmi);
#endif
}


int main()
{
    const size_t dataSize = 128*1024*1024;
    uint8_t* data = new uint8_t[dataSize];

    printf("Random data\n===========\n");
    std::mt19937 rand;
    for (size_t i=0; i<dataSize; ++i)
        data[i] = uint8_t(rand());
    bench_all(data, dataSize);

    printf("\nAll zeroes\n==========\n");
    memset(data, 0, dataSize);
    bench_all(data, dataSize);

    printf("\nSequence\n========\n");
    for (size_t i=0; i<dataSize; ++i)
        data[i] = uint8_t(i);
    bench_all(data, dataSize);

    return EXIT_SUCCESS;
}
