#include <iostream>
#include "order.hpp"
#include "allocator.hpp"

int main() {
    PoolAllocator<Order, 1000> pool;

    Order* o = pool.allocate();
    new (o) Order(1, 100.5, 10, true);

    std::cout << "ID: " << o->id << "\n";
    std::cout << "Price: " << o->price << "\n";
    std::cout << "Quantity: " << o->quantity << "\n";
    std::cout << "Buy: " << o->is_buy << "\n";

    o->~Order();
    pool.deallocate(o);

    std::cout << "Deallocated successfully\n";
}