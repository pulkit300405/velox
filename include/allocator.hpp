#pragma once

#include <cstddef>
#include <cassert>

template<typename T, std::size_t BlockCount>
class PoolAllocator {
private:
    struct Slot {
        alignas(T) std::byte data[sizeof(T)];
        Slot* next;
    };

    Slot pool[BlockCount];
    Slot* free_head;

public:
    PoolAllocator() {
        for (std::size_t i = 0; i < BlockCount - 1; i++) {
            pool[i].next = &pool[i + 1];
        }
        pool[BlockCount - 1].next = nullptr;
        free_head = &pool[0];
    }

    T* allocate() {
        assert(free_head != nullptr && "Pool exhausted");
        Slot* slot = free_head;
        free_head = free_head->next;
        return reinterpret_cast<T*>(slot->data);
    }

    void deallocate(T* ptr) {
        Slot* slot = reinterpret_cast<Slot*>(ptr);
        slot->next = free_head;
        free_head = slot;
    }
};