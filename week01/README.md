# Week 1 — Systems Foundations

First week learning C++ and systems programming from scratch. 
Coming from a Python/ML background so everything here was new.

## What I built

- `tensor.cpp` — a Tensor class wrapping a flat vector of floats, 
  with vector addition and timing benchmarks
- `matmul.cpp` — naive matrix multiply with three nested loops, 
  benchmarked at N=64 and N=256

## Benchmark results

**Vector add:**
| Size | Time |
|------|------|
| 1K   | 0.04 ms |
| 1M   | 23.8 ms |
| 10M  | 151.4 ms |

**Matrix multiply:**
| N   | Time |
|-----|------|
| 64  | 3.3 ms |
| 256 | 84.7 ms |

## What the numbers tell me

Vector add doesn't scale linearly — the jump from 1K to 1M is 
disproportionately large because small data fits in CPU cache and 
large data doesn't. Once you exceed cache, you're hitting RAM and 
it slows down hard.

Matrix multiply scales roughly as N³. N grew 4x, time grew 25x. 
At N=4096 this implementation would be unusable.

## What surprised me

Loop order and memory layout matter more than I expected. 
Also how immediately a double-delete crashes a program — 
that's a class of bug that just doesn't exist in Python.

## What's next

Week 2: fix the matmul by reordering the loops. Same algorithm, 
different memory access pattern. Expecting 2-5x speedup without 
changing any math.