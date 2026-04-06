# Week 4 — Multithreading and Parallel Matrix Multiply

## What I Built
- First threaded program showing non-deterministic execution order
- Parallel matrix multiply splitting rows across threads
- Scaling benchmark from 1 to 8 threads at N=1024

## Benchmark Results (O2, Apple M-series, N=1024)

| Version          | Time     | Speedup |
|------------------|----------|---------|
| Naive (i,j,k)    | 998ms    | 1x      |
| Reordered 1 thread | 72ms   | 13.9x   |
| 2 threads        | 43ms     | 1.7x    |
| 4 threads        | 23ms     | 3.1x    |
| 8 threads        | 15ms     | 4.5x    |

Note: the jump from naive to 1 thread is algorithm improvement 
(reordered loops), not parallelism. Real parallel scaling starts 
from the 1 thread baseline.

## Why Scaling Isn't Linear

8 threads gave 4.5x speedup, not 8x. Each doubling gave less:

1→2 threads: 1.7x
2→4 threads: 1.9x
4→8 threads: 1.5x  - here we see diminishing returns 

The bottleneck is memory bandwidth not compute. All 8 threads 
share the same memory bus — they're fighting over the same pipe 
to RAM. Once you saturate memory bandwidth, adding more cores 
just adds contention without adding throughput.

## What I Learned

**Non-determinism:** threads execute in arbitrary order decided 
by the OS. Running the same program twice gives different print 
order. This is fine when threads own separate memory regions — 
dangerous when they share state.

**Why our matmul is safe:** each thread owns completely separate 
rows. Thread 0 writes rows 0-255, thread 1 writes rows 256-511. 
No overlap, no locks needed.

**Race conditions:** if two threads wrote to the same memory 
location simultaneously you'd get corrupted results. One thread's 
write silently overwrites the other's. The correct answer would 
be wrong with no error or crash.

**std::ref():** threads copy arguments by default. You need 
std::ref() to force pass-by-reference, otherwise you're copying 
entire matrices into each thread which is expensive and wrong.

## The Pattern Across 4 Weeks

Every week points to the same conclusion:

- Week 2: memory access patterns dominate performance
- Week 3: hardware prefetcher on Apple Silicon already optimizes sequential access
- Week 4: memory bandwidth limits parallel scaling

Compute is cheap. Memory is the bottleneck. This is true on 
a laptop matmul and on A100s running transformer inference.

## What's Next

Week 5: benchmarking rigor. Right now my numbers have variance 
and noise. Learn to make results statistically credible — the 
difference between an experiment and a real measurement.

