# Velox

Low-latency quantitative order matching engine in C++20. Built around zero heap allocation in the hot path, price-time priority matching, and real-time trading signals — the same principles used in production HFT systems.

## How it works

Incoming orders go through a pool allocator instead of the system heap, land in a price-sorted order book, get matched against the opposite side, and update live trading signals on every trade.

## Components

### Pool Allocator
- Pre-allocates a fixed memory block at startup — no OS calls during trading
- Raw `std::byte` storage with `alignas` — no constructors called on pool init
- Placement new separates allocation from object construction
- O(1) allocate and deallocate via free list
- 1.6x faster than `new`/`delete` on fixed-size workloads

### Order Book
- Bids: `std::map<double, uint32_t, std::greater<double>>` — highest price first
- Asks: `std::map<double, uint32_t>` — lowest price first
- Match fires when best bid >= best ask
- Trades the minimum of bid and ask quantity, removes exhausted levels

### Signals
- **VWAP** — volume weighted average price across all matched trades
- **OFI** — order flow imbalance = (buy_vol - sell_vol) / (buy_vol + sell_vol), range [-1, +1], measures buy vs sell pressure
- **PnL** — tracks cash position, inventory, unrealized PnL at current market price, and total PnL

## Benchmark

1,000,000 allocations on Apple M-series:

| Method     | Total         | Per op |
|------------|---------------|--------|
| Pool       | 18,078,166 ns | 18 ns  |
| new/delete | 28,689,208 ns | 28 ns  |

Pool allocator is **1.6x faster** than the system allocator on fixed-size order objects.

## Project Structure

```
velox/
├── include/
│   ├── order.hpp       # Order struct — id, price, quantity, side
│   ├── allocator.hpp   # Pool allocator — templated, fixed capacity
│   ├── book.hpp        # Order book — price-time matching engine
│   └── signals.hpp     # VWAP, OFI, PnL
├── src/
│   └── main.cpp        # Demo
├── bench/
│   └── benchmark.cpp   # Pool vs new/delete benchmark
└── CMakeLists.txt
```

## Build

```bash
mkdir build && cd build
cmake ..
cmake --build .
./velox        # runs order book demo
./benchmark    # runs allocator benchmark
```

## Stack

C++20 · CMake · no external dependencies