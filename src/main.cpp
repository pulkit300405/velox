#include <iostream>
#include "order.hpp"

int main() {
    Order o(1, 100.5, 10, true);

    std::cout << "ID: " << o.id << "\n";
    std::cout << "Price: " << o.price << "\n";
    std::cout << "Quantity: " << o.quantity << "\n";
    std::cout << "Buy: " << o.is_buy << "\n";
    std::cout << "Size: " << sizeof(Order) << " bytes\n";
}