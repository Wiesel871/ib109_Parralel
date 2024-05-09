#ifndef PX
#define PX

#include <cstddef>
#include <pthread.h>
#include <queue>

#include "void.h"

namespace px {
    struct pxq : abstract_queue {
        std::queue<std::size_t> q;
        pthread_mutex_t m = pthread_mutex_t{};

        bool isEmpty() override;
        void enqueue(std::size_t) override;
        void dequeue() override;
    };
}
#endif
