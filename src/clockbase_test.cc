#include <unistd.h>
#include <chrono>
#include "catch.hpp"
#include "easylogging++.h"
#include "clockbase.h"
#include "timer.h"

TEST_CASE( "clockbase init", "[clockbase]") {
    clockbase c;
    LOG(INFO) << c.clock_ms();
    sleep(1);
    LOG(INFO) << c.clock_ms();
}

TEST_CASE( "clockevent init", "[clockevent]") {
    auto cb = [](void) {
        LOG_EVERY_N(1000, INFO) << "timer callback" << std::endl;
    };

    clockbase c(cb);
    std::this_thread::sleep_for(std::chrono::seconds(5));
}

TEST_CASE( "timer init", "[timer]") {
    scheduler s;
    auto cb1 = [](void) {
        LOG(INFO) << "hanxu timer1 callback at: "<<clockbase::clock_ms()<<"ms"<< std::endl;
    };
    auto cb2 = [](void) {
        LOG(INFO) << "hanxu timer2 callback at: "<<clockbase::clock_ms()<<"ms"<< std::endl;
    };
    auto t1 = s.add_timer(100, 100, cb1);
    auto t2 = s.add_timer(200, 200, cb2);
    std::this_thread::sleep_for(std::chrono::seconds(5));
}
