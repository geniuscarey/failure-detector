#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include "transport.h"
#include "timer.h"
#include "easylogging++.h"

transport::transport(short port) {
    sockaddr_in myaddr;
    int fd;
    int recvlen;
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        LOG(ERROR) <<"create socket failed:"<<std::strerror(errno)<< std::endl;
        return;
    }
    
    //TODO, error handle
    int flags = fcntl(fd, F_GETFL, 0); 
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);

    memset((char *)&myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myaddr.sin_port = htons(port);

    if (bind(fd, (struct sockaddr *)&myaddr,sizeof(myaddr))) {
        LOG(ERROR) <<"bind socket failed:"<<std::strerror(errno)<< std::endl;
        return;
    }
}

transport::rx_burst(int num) {
}

transport::~transport() {
    ticker_->cancel();
}
