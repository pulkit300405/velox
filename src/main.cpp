#include <iostream>
#include "order.hpp"
#include "allocator.hpp"
#include "book.hpp"

int main() {
    OrderBook book;

    book.addOrder(100.50, 500, true);
    book.addOrder(100.25, 200, true);
    book.addOrder(101.00, 300, false);
    book.addOrder(101.25, 100, false);
    book.print();

    std::cout << "\nAdding matching order...\n";
    book.addOrder(100.50, 300, false);
    book.print();
}