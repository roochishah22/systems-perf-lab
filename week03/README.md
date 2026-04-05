# Week 3 — Profiling and Optimization

## What I Built
- Benchmark harness with a reusable timing function using lambdas
- Four matmul implementations: naive, reordered, inlined, tiled
- Systematic comparison across O0/O1/O2/O3 compiler flags

## Benchmark Results (O2, Apple M-series)

| N    | Naive    | Reordered | Inlined  | Tiled 16 | Tiled 32 |
|------|----------|-----------|----------|----------|----------|
| 512  | 128ms    | 8.9ms     | 8.9ms    | 20ms     | 12ms     |
| 1024 | 986ms    | 71ms      | 71ms     | 190ms    | 116ms    |
| 2048 | 11192ms  | 577ms     | 575ms    | 2107ms   | 1168ms   |

## Compiler Flag Experiment (N=512, reordered)

| Flag | Time     |
|------|----------|
| O0   | 753ms    |
| O1   | 36ms     |
| O2   | 8.9ms    |
| O3   | 9.4ms    |

Biggest jump was O0→O1. Cause: inlining eliminated ~400 million 
at() function calls.

## What I Learned

**Memory layout:** matrices are stored as flat strips in memory. 
Row elements are contiguous, column elements are N positions apart. 
Accessing columns causes cache misses on every iteration — the CPU 
has to go back to RAM each time. Accessing rows is sequential so 
the CPU preloads whole rows into cache automatically.

**Inlining:** the at() method looked simple but calling it 400 
million times at O0 meant the CPU was constantly jumping to the 
function definition and back — like flipping to page 50 of a recipe 
book on every step. Bypassing it with direct data[] access gave 40% 
speedup at O0 with zero algorithmic change.

**Loop tiling:** the idea is correct — break the matrix into small 
tiles that fit in cache, load each tile once, use every element 
fully, move on. At O0 tiling wins at large N, proving the intuition 
is sound. At O2 it loses because Apple Silicon's hardware prefetcher 
already handles sequential access patterns automatically, making 
the overhead of six nested loops not worth it.

## Key Insight

On Apple Silicon, the hardware prefetcher is aggressive enough to 
make reordered matmul near-optimal without explicit tiling. Real 
gains on this architecture require SIMD vectorization — processing 
4-8 floats simultaneously — not just cache blocking. That's Week 4.

## What's Next

SIMD and vectorization — making the CPU process multiple floats 
at once instead of one at a time.