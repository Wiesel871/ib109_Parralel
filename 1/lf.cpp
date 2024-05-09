#include "lf.h"
#include <cassert>
#include <cmath>
#include <cstddef>
#include <pthread.h>
#include <atomic>

bool lf::lfq::isEmpty() {
    int expected = 0;
    while (!u.compare_exchange_strong(expected, 1)) {
        expected = 0;
    };
    assert(u);
    bool res = q.empty();
    u.store(0);
    return res;
}

void lf::lfq::enqueue(std::size_t x) {
    int expected = 0;
    while (!u.compare_exchange_strong(expected, 1)) {
        expected = 0;
    };
    assert(u);
    q.push(x);
    u.store(0);
}

void lf::lfq::dequeue() {
    int expected = 0;
    while (!u.compare_exchange_strong(expected, 1)) {
        expected = 0;
    }
    assert(u);
    if (!q.empty())
        q.pop();
    u.store(0);
}
