// Measuring bandwidth from different cache levels
// $ g++ -std=c++11 -O3 -march=native bandwidth-vectorized.cpp && ./a.out -n 400000000

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <immintrin.h>
#include "utils.h"

int main(int argc, char** argv) {
  Timer t;
  long n = read_option<long>("-n", argc, argv);
  long repeat = 1e9 / n;
  assert(n % 64 == 0);

  double* x = (double*) aligned_malloc(n * sizeof(double));
  for (long i = 0; i < n; i++) x[i] = i+1;

  t.tic();
  for (long k = 0; k < repeat; k++) {
    __m256d kk = _mm256_set_pd(k,k,k,k);
    for (long i = 0; i < n; i += 64) {
      _mm256_stream_pd(x+i+ 0, kk);
      _mm256_stream_pd(x+i+ 4, kk);
      _mm256_stream_pd(x+i+ 8, kk);
      _mm256_stream_pd(x+i+12, kk);
      _mm256_stream_pd(x+i+16, kk);
      _mm256_stream_pd(x+i+20, kk);
      _mm256_stream_pd(x+i+24, kk);
      _mm256_stream_pd(x+i+28, kk);
      _mm256_stream_pd(x+i+32, kk);
      _mm256_stream_pd(x+i+36, kk);
      _mm256_stream_pd(x+i+40, kk);
      _mm256_stream_pd(x+i+44, kk);
      _mm256_stream_pd(x+i+48, kk);
      _mm256_stream_pd(x+i+52, kk);
      _mm256_stream_pd(x+i+56, kk);
      _mm256_stream_pd(x+i+60, kk);
    }
  }
  printf("time to non-temp write = %8.2f s    ", t.toc());
  printf("bandwidth = %8.2f GB/s\n", n * repeat * sizeof(double) / 1e9 / t.toc());

  t.tic();
  for (long k = 0; k < repeat; k++) {
    __m256d kk = _mm256_set_pd(k,k,k,k);
    for (long i = 0; i < n; i += 64) {
      _mm256_store_pd(x+i+ 0, kk);
      _mm256_store_pd(x+i+ 4, kk);
      _mm256_store_pd(x+i+ 8, kk);
      _mm256_store_pd(x+i+12, kk);
      _mm256_store_pd(x+i+16, kk);
      _mm256_store_pd(x+i+20, kk);
      _mm256_store_pd(x+i+24, kk);
      _mm256_store_pd(x+i+28, kk);
      _mm256_store_pd(x+i+32, kk);
      _mm256_store_pd(x+i+36, kk);
      _mm256_store_pd(x+i+40, kk);
      _mm256_store_pd(x+i+44, kk);
      _mm256_store_pd(x+i+48, kk);
      _mm256_store_pd(x+i+52, kk);
      _mm256_store_pd(x+i+56, kk);
      _mm256_store_pd(x+i+60, kk);
    }
  }
  printf("time to write          = %8.2f s    ", t.toc());
  printf("bandwidth = %8.2f GB/s\n", n * repeat * sizeof(double) / 1e9 / t.toc());

  t.tic();
  for (long k = 0; k < repeat; k++) {
    __m256d half = _mm256_set_pd(0.5,0.5,0.5,0.5);
    __m256d kk = _mm256_set_pd(x[0]*0.5,x[0]*0.5,x[0]*0.5,x[0]*0.5);
    for (long i = 0; i < n; i += 64) {
      _mm256_store_pd(x+i+ 0, _mm256_fmadd_pd(_mm256_load_pd(x+i+ 0), half, kk));
      _mm256_store_pd(x+i+ 4, _mm256_fmadd_pd(_mm256_load_pd(x+i+ 4), half, kk));
      _mm256_store_pd(x+i+ 8, _mm256_fmadd_pd(_mm256_load_pd(x+i+ 8), half, kk));
      _mm256_store_pd(x+i+12, _mm256_fmadd_pd(_mm256_load_pd(x+i+12), half, kk));
      _mm256_store_pd(x+i+16, _mm256_fmadd_pd(_mm256_load_pd(x+i+16), half, kk));
      _mm256_store_pd(x+i+20, _mm256_fmadd_pd(_mm256_load_pd(x+i+20), half, kk));
      _mm256_store_pd(x+i+24, _mm256_fmadd_pd(_mm256_load_pd(x+i+24), half, kk));
      _mm256_store_pd(x+i+28, _mm256_fmadd_pd(_mm256_load_pd(x+i+28), half, kk));
      _mm256_store_pd(x+i+32, _mm256_fmadd_pd(_mm256_load_pd(x+i+32), half, kk));
      _mm256_store_pd(x+i+36, _mm256_fmadd_pd(_mm256_load_pd(x+i+36), half, kk));
      _mm256_store_pd(x+i+40, _mm256_fmadd_pd(_mm256_load_pd(x+i+40), half, kk));
      _mm256_store_pd(x+i+44, _mm256_fmadd_pd(_mm256_load_pd(x+i+44), half, kk));
      _mm256_store_pd(x+i+48, _mm256_fmadd_pd(_mm256_load_pd(x+i+48), half, kk));
      _mm256_store_pd(x+i+52, _mm256_fmadd_pd(_mm256_load_pd(x+i+52), half, kk));
      _mm256_store_pd(x+i+56, _mm256_fmadd_pd(_mm256_load_pd(x+i+56), half, kk));
      _mm256_store_pd(x+i+60, _mm256_fmadd_pd(_mm256_load_pd(x+i+60), half, kk));
    }
  }
  printf("time to read + write   = %8.2f s    ", t.toc());
  printf("bandwidth = %8.2f GB/s\n", 2 * n * repeat * sizeof(double) / 1e9 / t.toc());

  t.tic();
  __m256d sum[16];
  for (int i = 0; i < 16; i++) sum[i] = _mm256_set_pd(0,0,0,0);
  for (long k = 0; k < repeat; k++) {
    for (long i = 0; i < n; i += 64) {
      sum[ 0] = _mm256_add_pd(_mm256_load_pd(x+i+ 0), sum[ 0]);
      sum[ 1] = _mm256_add_pd(_mm256_load_pd(x+i+ 4), sum[ 1]);
      sum[ 2] = _mm256_add_pd(_mm256_load_pd(x+i+ 8), sum[ 2]);
      sum[ 3] = _mm256_add_pd(_mm256_load_pd(x+i+12), sum[ 3]);
      sum[ 4] = _mm256_add_pd(_mm256_load_pd(x+i+16), sum[ 4]);
      sum[ 5] = _mm256_add_pd(_mm256_load_pd(x+i+20), sum[ 5]);
      sum[ 6] = _mm256_add_pd(_mm256_load_pd(x+i+24), sum[ 6]);
      sum[ 7] = _mm256_add_pd(_mm256_load_pd(x+i+28), sum[ 7]);
      sum[ 8] = _mm256_add_pd(_mm256_load_pd(x+i+32), sum[ 8]);
      sum[ 9] = _mm256_add_pd(_mm256_load_pd(x+i+36), sum[ 9]);
      sum[10] = _mm256_add_pd(_mm256_load_pd(x+i+40), sum[10]);
      sum[11] = _mm256_add_pd(_mm256_load_pd(x+i+44), sum[11]);
      sum[12] = _mm256_add_pd(_mm256_load_pd(x+i+48), sum[12]);
      sum[13] = _mm256_add_pd(_mm256_load_pd(x+i+52), sum[13]);
      sum[14] = _mm256_add_pd(_mm256_load_pd(x+i+56), sum[14]);
      sum[15] = _mm256_add_pd(_mm256_load_pd(x+i+60), sum[15]);
    }
  }
  __m256d sum_ = _mm256_set_pd(0,0,0,0);
  for (int i = 0; i < 16; i++) sum_ = _mm256_add_pd(sum_, sum[i]);
  printf("time to read           = %8.2f s    ", t.toc());
  printf("bandwidth = %8.2f GB/s\n", n * repeat * sizeof(double) / 1e9 / t.toc());

  aligned_free(x);

  return sum_[0]+sum_[1]+sum_[2]+sum_[3];
}

