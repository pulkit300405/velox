#include <iostream>
#include <chrono>
#include <memory>
#include "order.hpp"
#include "allocator.hpp"

static const int N = 1'000'000;

void benchPool() {
    auto pool = std::make_unique<PoolAllocator<Order, 1'000'000>>();

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < N; i++) {
        Order* o = pool->allocate();
        new (o) Order(i, 100.0, 10, true);
        o->~Order();
        pool->deallocate(o);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

    std::cout << "Pool:       " << ns << " ns total | " << ns / N << " ns/op\n";
}

void benchNew() {
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < N; i++) {
        Order* o = new Order(i, 100.0, 10, true);
        delete o;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

    std::cout << "new/delete: " << ns << " ns total | " << ns / N << " ns/op\n";
}

int main() {
    std::cout << "=== BENCHMARK: Pool vs new/delete ===\n";
    std::cout << "N = " << N << " allocations\n\n";
    benchPool();
    benchNew();
}