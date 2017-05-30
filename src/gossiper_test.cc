#include "catch.hpp"
#include "gossiper.h"
#include <event2/event.h>

static void timeout_cb(evutil_socket_t fd, short event, void *arg)
{
    struct timeval newtime, difference;
    gossiper *pg = (gossiper *)arg;
    pg->probe();
    struct timeval five_seconds = {1,0};
    event_add(&pg->fd, &tv);
}
TEST_CASE( "gossiper init", "[gossiper]" ) {
    struct event timeout;
    struct timeval five_seconds = {1,0};

    int flags = 0;
    struct event_base *base = event_base_new();
    gossiper g;
    event_assign(
            &timeout, base, -1, flags, 
            timeout_cb, (void*) &timeout);
    event_add(&timeout, &tv);
    REQUIRE( g.nodes.size() == 0);
}
