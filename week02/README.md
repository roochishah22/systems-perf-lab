# Week 2 — Cache Locality

## What I Built
- Matrix struct with flat vector storage and at(i,j) accessor
- Naive matrix multiply (i,j,k loop order) — correct but cache-unfriendly
- Reordered matrix multiply (i,k,j loop order) — same math, better memory access
- Cache demo showing sequential vs strided access patterns

## Benchmark Results

**Without optimization flag:**
| N   | Naive    | Reordered |
|-----|----------|-----------|
| 256 | 114.4 ms | 95.3 ms   |
| 512 | 751.9 ms | 753.6 ms  |

**With -O2 optimization flag:**
| N   | Naive   | Reordered | Speedup |
|-----|---------|-----------|---------|
| 256 | 20.8 ms | 1.8 ms    | 11x     |
| 512 | 125.9 ms| 9.0 ms    | 14x     |

## What the numbers tell me

The naive version accesses B.at(k,j) jumping down columns in memory 
on every inner loop iteration. Columns are not contiguous in memory 
so every access is a cache miss — the CPU stalls waiting for data 
to arrive from RAM.

The reordered version increments j in the innermost loop, moving 
across a row which is sequential in memory. The CPU preloads the 
whole row into cache and reuses it. Near zero cache misses.

Same arithmetic. Same data. 14x faster. Pure memory access pattern.

Also notable: -O2 alone took naive from 114ms to 20ms. Always 
benchmark with -O2 otherwise you're measuring compiler laziness, 
not your algorithm.

## What's next

Week 3: profiling. Instead of just observing performance differences,
learn to diagnose exactly where time is going and prove it with data.