#include <iostream>
#include "gossiper.h"
#include "easylogging++.h"

void gossiper::probe() {
    int num_check = 0;
    LOG(INFO) << "probe start" << num_check;

    do {
        if (this->probe_index >= this->nodes.size()) {
            this->probe_index = 0;
            num_check++;
        }

        node_state *n = this->nodes[this->probe_index];
        
        this->probe_index++;
        if ((n->name == this->name) || (n->status == NODE_DEAD )) {
            num_check++;
            continue;
        }
        
        this->probe_node(n);
    } while(num_check < this->nodes.size());
}

void gossiper::probe_timeout(node_state *pstate) {

}

void gossiper::probe_node(node_state *pstate) {
    LOG(INFO) << "do nothing";
    
    if (pstate->status == NODE_ALIVE) {
        //TODO, send ping msg
    } else {
        //TODO, send ping msg pack with suspect msg
    }
    
    //TODO, set probe timeout func
    
    return
}
