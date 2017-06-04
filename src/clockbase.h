#pragma once
#include <chrono>
#include <thread>
#include <atomic>
#include <functional>
#include <stdint.h>

using namespace std::chrono;

class clockbase;

using clock_handler = std::function<void (void)>;

class clockbase{
public:
    clockbase(int resolution, clock_handler h);
    clockbase(clock_handler h);
    clockbase();
    ~clockbase();
    static uint64_t clock_ms() {
        return (duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch())).count();
    }
private:
    int resolution_;
    clock_handler handler_;
    std::atomic<bool> running_{};
    std::thread thread_;
    void loop();
};
