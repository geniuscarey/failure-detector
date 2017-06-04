#include "timer.h"

class transport {
public:
    transport(short);
    ~transport();

    void rx_burst(int num);
    void register_handler(void);
private:
    timerptr ticker_;
};

class udp_transport {
};
