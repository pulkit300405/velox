#pragma once

#include <cstdint>

struct Order {
    uint64_t id;
    double price;
    uint32_t quantity;
    bool is_buy;

    Order(uint64_t i, double p, uint32_t q, bool b)
        : id(i), price(p), quantity(q), is_buy(b) {}
};