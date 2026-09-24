# Velox

Low-latency quantitative order matching engine in C++20, built around the principles used in HFT systems — zero heap allocation in the hot path, cache-friendly memory layout, and real-time quantitative signals.

## Motivation

Standard allocators are general purpose. In latency-sensitive systems, every `new`/`delete` call hits the OS, acquires locks, and fragments memory. Velox eliminates this by managing memory explicitly via a pool allocator, keeping the critical path allocation-free.

## Architecture

```
Incoming Order
      ↓
Pool Allocator  ←  pre-allocated fixed memory block
      ↓
Order Book      ←  price-time priority matching
      ↓
Match Engine    ←  best bid vs best ask
      ↓
Signals         ←  VWAP, OFI, PnL
```

## Components

### Pool Allocator
- Fixed-size memory pool using raw `std::byte` storage with `alignas`
- Placement new for construction — decouples allocation from object lifetime
- O(1) allocate and deallocate via free list
- No OS calls, no locks, no fragmentation

### Order Book
- Bids: `std::map<double, uint32_t, std::greater<double>>` — highest price first
- Asks: `std::map<double, uint32_t>` — lowest price first
- Match condition: best bid >= best ask
- Trades minimum of bid and ask quantity

### Signals
- **VWAP** — Volume Weighted Average Price across all matched trades
- **OFI** — Order Flow Imbalance = (buy_vol - sell_vol) / (buy_vol + sell_vol), range [-1, +1]
- **PnL** — cash position, inventory, unrealized PnL at current price, total PnL

## Benchmark

1,000,000 allocations on Apple M-series:

| Method     | Total         | Per op |
|------------|---------------|--------|
| Pool       | 18,078,166 ns | 18 ns  |
| new/delete | 28,689,208 ns | 28 ns  |

**1.6x faster** than system allocator on fixed-size workloads.

## Project Structure

```
velox/
├── include/
│   ├── order.hpp       # Order struct — id, price, quantity, side
│   ├── allocator.hpp   # Pool allocator — templated, fixed capacity
│   ├── book.hpp        # Order book — matching engine
│   └── signals.hpp     # VWAP, OFI, PnL
├── src/
│   └── main.cpp        # Demo
├── bench/
│   └── benchmark.cpp   # Pool vs new/delete
└── CMakeLists.txt
```

## Build

```bash
mkdir build && cd build
cmake ..
cmake --build .
./velox        # order book demo
./benchmark    # allocator benchmark
```

## Stack

C++20 · CMake · no external dependencies