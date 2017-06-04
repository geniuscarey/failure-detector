#pragma once
#include <map>
#include <memory>
#include <functional>
#include "clockbase.h"
#include "easylogging++.h"

class timer;
using timer_cb = std::function<void (void)>; 

class timer: public el::Loggable{
public:
    timer(uint32_t interval, timer_cb cb);

    void change(uint32_t delay, uint32_t interval) {};
    void cancel() {
        active_ = false;
    }
    
    uint32_t period() {
        return interval_;
    }
    
    bool active() {
        return active_;
    }

    virtual void log(el::base::type::ostream_t& os) const {
        os<<" timer<"<<id_<<"> "<<delay_<<" "<<interval_<<" "<<active_<<std::endl;
    }

    timer_cb cb;

private:
    uint32_t delay_;
    uint32_t interval_;
    uint64_t id_;
    bool active_;
};

using timerptr = std::shared_ptr<timer>; 

class scheduler{
    public:
        scheduler();
        ~scheduler();

        void tick();
        timerptr add_timer(uint32_t delay, uint32_t interval, timer_cb cb);
    private:
        clockbase *clockbase_;
        std::multimap<uint64_t, timerptr> timer_map_;
};

