#include <unistd.h>
#include <sys/epoll.h>
#include <sys/timerfd.h>
#include <assert.h>

#include "clockbase.h"
#include "easylogging++.h"

clockbase::clockbase(int resolution, clock_handler h): resolution_(resolution), handler_(h){
    running_ = true;
    thread_ = std::thread([this]() {
            this->loop();
    });
};

clockbase::clockbase(clock_handler h): resolution_(1),handler_(h){
    running_ = true;
    thread_ = std::thread([this]() {
            this->loop();
    });
}

clockbase::clockbase(): resolution_(1),handler_(nullptr) {
    running_ = true;
    thread_ = std::thread([this]() {
            this->loop();
    });
}

void clockbase::loop() {
	int efd = epoll_create1(EPOLL_CLOEXEC);
	struct epoll_event ev;

	int tfd;//timer fd

	if((tfd= timerfd_create(CLOCK_MONOTONIC,TFD_NONBLOCK)) < 0) {
		LOG(ERROR) <<"timerfd create error"<<std::endl;
    }

	struct itimerspec its;
	memset(&its, 0, sizeof(its));  

	struct timespec ts;
	ts.tv_sec = resolution_/1000;
	ts.tv_nsec = resolution_%1000*1000000;

	//both interval and value have been set
	its.it_value = ts; 
	its.it_interval = ts;

	if( timerfd_settime(tfd, 0, &its, NULL) <0)
	{
		LOG(ERROR) <<"settime error"<<strerror(errno)<< std::endl;
	}   

	ev.data.fd = tfd;
	ev.events = EPOLLIN | EPOLLET;

	if( epoll_ctl(efd,EPOLL_CTL_ADD,tfd,&ev) < 0)
		LOG(ERROR) <<"epoll_ctl error"<<strerror(errno)<<std::endl;

	while(running_) {
		if(int nfds = epoll_wait(efd, &ev, 1, 1000) > 0) {
			for (int i=0;i<nfds;i++)
			{
				if(ev.data.fd == tfd)
				{
                    uint64_t discard;
                    read(tfd, &discard, sizeof(discard));
                    LOG(DEBUG) << "clockbase timeout, callback at " << clockbase::clock_ms() << std::endl;
                    if (handler_) {
                        handler_();
                    }
				}
			}       
		}
	}

    close(efd);
    close(tfd);
}

clockbase::~clockbase() {
    running_ = false;
    std::cout << "clockbase::~clockbase running set to: " << running_ << std::endl;
    if (thread_.joinable()) {
        thread_.join();
    }
}
