#include <vector>
#include <atomic>
#include "clockbase.h"
#include "timer.h"
#include "easylogging++.h"

static uint64_t g_timerid = 0;

timer::timer(uint32_t interval, timer_cb cb):
    interval_(interval), cb(cb), active_(true), id_{++g_timerid} {
};

scheduler::scheduler() {
    auto cb = [this] () {
        this->tick(); 
    };

    clockbase_  = new clockbase(cb);
}

scheduler::~scheduler() {
    delete clockbase_;
}

void scheduler::tick() {
    LOG(DEBUG) <<"scheduler::tick at "<<clockbase::clock_ms()<<std::endl;
    std::vector<timerptr> readd;
    auto it = timer_map_.cbegin();
    auto now = clockbase::clock_ms();
    while (it != timer_map_.cend() && it->first <= now) {
        auto pt = it->second;
        it = timer_map_.erase(it);
        if (!pt->active()) {
            continue;
        }

        pt->cb();
        // if timer is periodic, readd it
        if (pt->period()) {
            //LOG(INFO) <<"periodic timer, readd it"<<std::endl;
            readd.push_back(pt);
        }
    }
    
    for (auto pt: readd) {
        LOG(DEBUG) <<"readd periodic timer"<<*pt<<" at "<<now<<std::endl;
        timer_map_.insert(std::make_pair(now+pt->period(),pt));
    }
}

timerptr scheduler::add_timer(uint32_t delay, uint32_t interval, timer_cb cb) {
    auto now = clockbase::clock_ms();
    auto ptimer = std::make_shared<timer>(interval,cb);
    auto it = timer_map_.insert(std::make_pair(now+delay, ptimer));
    LOG(DEBUG) <<"scheduler:add_timer: "<<it->second<<" at: " <<now<< std::endl;
    return  ptimer;
}
