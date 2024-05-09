#ifndef VOID
#define VOID

#include <cstddef>
struct abstract_queue {
    virtual bool isEmpty() = 0;
    virtual void enqueue(std::size_t) = 0;
    virtual void dequeue() = 0;
};
#endif
