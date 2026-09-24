#pragma once

#include <cstdint>
#include <iostream>

class Signals {
private:
    double total_pv = 0.0;
    double total_vol = 0.0;

    double buy_vol = 0.0;
    double sell_vol = 0.0;

    double cash = 0.0;
    int32_t inventory = 0;
    double realized_pnl = 0.0;

public:
    void onTrade(double price, uint32_t qty, bool is_buy) {
        total_pv += price * qty;
        total_vol += qty;

        if (is_buy) {
            buy_vol += qty;
            cash -= price * qty;
            inventory += qty;
        } else {
            sell_vol += qty;
            cash += price * qty;
            inventory -= qty;
        }
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

    double unrealizedPnL(double current_price) const {
        return inventory * current_price;
    }

    double totalPnL(double current_price) const {
        return cash + unrealizedPnL(current_price);
    }

    void print(double current_price = 0.0) const {
        std::cout << "\n=== SIGNALS ===\n";
        std::cout << "VWAP:          " << vwap() << "\n";
        std::cout << "OFI:           " << ofi() << "\n";
        std::cout << "Cash:          " << cash << "\n";
        std::cout << "Inventory:     " << inventory << "\n";
        std::cout << "Unrealized PnL:" << unrealizedPnL(current_price) << "\n";
        std::cout << "Total PnL:     " << totalPnL(current_price) << "\n";
        std::cout << "===============\n";
    }
};