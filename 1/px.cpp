#include "px.h"
#include <cstddef>
#include <pthread.h>

bool px::pxq::isEmpty() {
    pthread_mutex_lock(&m);
    bool res = q.empty();
    pthread_mutex_unlock(&m);
    return res;
}

void px::pxq::enqueue(std::size_t x) {
    pthread_mutex_lock(&m);
    q.push(x);
    pthread_mutex_unlock(&m);
}

void px::pxq::dequeue() {
    pthread_mutex_lock(&m);
    if (!q.empty())
        q.pop();
    pthread_mutex_unlock(&m);
}
