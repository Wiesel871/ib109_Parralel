#ifndef LF 
#define LF

#include <cstddef>
#include <queue>
#include "void.h"
#include <atomic>

namespace lf {
    struct lfq : abstract_queue {
        std::queue<std::size_t> q;
        std::atomic_int u = 0;

        bool isEmpty() override;
        void enqueue(std::size_t) override;
        void dequeue() override;
    };
}


#endif
