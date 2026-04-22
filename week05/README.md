# Week 5 — Benchmarking Rigor

## Methodology
Each implementation was run 20 trials with 3 warmup runs discarded. 
Median runtime is reported as the primary metric. All results at 
N=1024 with -O2 optimization on Apple M-series. Fresh output matrix 
created for each version to avoid accumulation errors.

## Results (N=1024, Apple M-series, -O2)

| Version    | Min     | Median  | Max     | Speedup vs naive |
|------------|---------|---------|---------|-----------------|
| Naive      | 973ms   | 1004ms  | 1058ms  | 1x              |
| Reordered  | 71.7ms  | 71.9ms  | 75.2ms  | 14x             |
| Inlined    | 72.0ms  | 72.1ms  | 119ms   | 14x             |
| 1 thread   | 71.9ms  | 72.0ms  | 72.3ms  | 14x             |
| 2 threads  | 36.6ms  | 38.9ms  | 41.2ms  | 26x             |
| 4 threads  | 18.6ms  | 18.6ms  | 20.9ms  | 54x             |
| 8 threads  | 15.1ms  | 15.4ms  | 18.5ms  | 65x             |

## Observations

**Inlined has a suspicious outlier** — max of 119ms vs median of 
72ms. Without 20 trials this could have been reported as the result 
and drawn wrong conclusions. Single measurements are unreliable.

**Multithreading increases variance** — 8 thread spread is 3ms vs 
1 thread spread of 0.5ms. More threads means more OS scheduling 
noise. Faster but less predictable — important for latency-sensitive 
production systems.

**Reordered and 1 thread are identical** — 71.9ms vs 72.0ms median. 
Confirms they're the same algorithm, different code paths. The 
parallelism gain starts at 2 threads.

**Parallel scaling is memory-bound** — going from 1 to 8 threads 
gave 4.6x speedup not 8x. All threads share the same memory bus. 
Once memory bandwidth is saturated, adding cores adds contention 
not throughput.

## What changed from Week 4

Week 4 reported single-run numbers. Those numbers looked clean but 
were hiding noise. The inlined outlier (119ms) and threading variance 
only became visible with multiple trials. Real performance claims 
require distributions, not single measurements.

## Key Takeaway

Reporting a single runtime is a guess. Reporting min/median/max 
across 20 trials with controlled warmup is a measurement. The 
difference matters when you're making optimization decisions — 
you need to know if an improvement is real or just noise.

## What's Next

TinyInfer — a from-scratch transformer inference engine in C++. 
Applying everything from weeks 1-5 to a real ML system: memory 
layout, cache optimization, threading, and rigorous be