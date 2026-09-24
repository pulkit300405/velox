#pragma once

#include <map>
#include <iostream>
#include <cstdint>
#include "order.hpp"
#include "allocator.hpp"
#include "signals.hpp"

class OrderBook {
private:
    std::map<double, uint32_t, std::greater<double>> bids;
    std::map<double, uint32_t> asks;

    PoolAllocator<Order, 10000> pool;
    Signals signals;
    uint64_t next_id = 1;

public:
    void addOrder(double price, uint32_t qty, bool is_buy) {
        Order* o = pool.allocate();
        new (o) Order(next_id++, price, qty, is_buy);

        if (is_buy) {
            bids[price] += qty;
        } else {
            asks[price] += qty;
        }

        match();
    }

    void match() {
        while (!bids.empty() && !asks.empty()) {
            auto best_bid = bids.begin();
            auto best_ask = asks.begin();

            if (best_bid->first >= best_ask->first) {
                uint32_t traded = std::min(best_bid->second, best_ask->second);
                std::cout << "MATCH: " << traded << " @ " << best_ask->first << "\n";

                signals.onTrade(best_ask->first, traded, true);

                best_bid->second -= traded;
                best_ask->second -= traded;

                if (best_bid->second == 0) bids.erase(best_bid);
                if (best_ask->second == 0) asks.erase(best_ask);
            } else {
                break;
            }
        }
    }

    void print(double current_price = 0.0) {
        std::cout << "\n=== ORDER BOOK ===\n";
        std::cout << "SELL:\n";
        for (auto it = asks.rbegin(); it != asks.rend(); ++it)
            std::cout << "  " << it->first << " x " << it->second << "\n";
        std::cout << "--------\n";
        std::cout << "BUY:\n";
        for (auto& [price, qty] : bids)
            std::cout << "  " << price << " x " << qty << "\n";
        signals.print(current_price);
        std::cout << "==================\n";
    }
};