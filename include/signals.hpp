#pragma once

#include <cstdint>
#include <vector>

class Signals {
private:
    double total_pv = 0.0;  // price x volume
    double total_vol = 0.0; // total volume

    double buy_vol = 0.0;
    double sell_vol = 0.0;

public:
    void onTrade(double price, uint32_t qty, bool is_buy) {
        total_pv += price * qty;
        total_vol += qty;

        if (is_buy) buy_vol += qty;
        else sell_vol += qty;
    }

    double vwap() const {
        if (total_vol == 0) return 0.0;
        return total_pv / total_vol;
    }

    double ofi() const {
        double total = buy_vol + sell_vol;
        if (total == 0) return 0.0;
        return (buy_vol - sell_vol) / total;
    }

    void print() const {
        std::cout << "\n=== SIGNALS ===\n";
        std::cout << "VWAP: " << vwap() << "\n";
        std::cout << "OFI:  " << ofi() << "\n";
        std::cout << "===============\n";
    }
};