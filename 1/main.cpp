#include <cstddef>
#include <ctime>
#include <functional>
#include <iostream>
#include <thread>
#include <barrier>
#include <chrono>

#include "px.h"
#include "lf.h"
#include "void.h"

const std::size_t tcount = 4 + 1;

void benchmark(abstract_queue *q, std::barrier<std::function<void()>> &barrier) {
    barrier.arrive_and_wait();
    for (std::size_t i = 0; i < 10; ++i) {
        q->enqueue(0);
        q->enqueue(1);
        q->dequeue();
        q->enqueue(2);
        for (std::size_t j = 0; j < 1000; ++j)
            q->enqueue(j);
        while (!q->isEmpty())
            q->dequeue();
    }
    barrier.arrive_and_wait();
}

int main() {
    std::cout << "536388\n";
    lf::lfq lf;
    px::pxq px;

    std::chrono::milliseconds t;
    bool start = true;
    std::function<void()> manager = [&]() noexcept {
        if (start) {
            start = false;
            t = duration_cast< std::chrono::milliseconds >(
                    std::chrono::system_clock::now().time_since_epoch()
            );
        }
        else {
            start = true;
            t = duration_cast< std::chrono::milliseconds >(
                    std::chrono::system_clock::now().time_since_epoch()
            ) - t;
        }
    };

    std::vector<std::jthread> threads(0);

    std::barrier<std::function<void()>> barrier(tcount, manager);

    for (std::size_t i = 0; i < tcount - 1; ++i) {
        threads.emplace_back(benchmark, &lf, std::ref(barrier));
    }
    barrier.arrive_and_wait();

    barrier.arrive_and_wait();

    threads.clear();
    std::chrono::milliseconds lf_time = t;


    for (std::size_t i = 0; i < tcount - 1; ++i)
        threads.emplace_back(benchmark, &px, std::ref(barrier));
    barrier.arrive_and_wait();

    barrier.arrive_and_wait();

    threads.clear();
    std::chrono::milliseconds px_time = t;

    std::cout << (lf_time * 100) / px_time << std::endl;

    return 0;
}
