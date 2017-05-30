#include <stdint.h>
#include <vector>
#include <map>
#include <string>

#define MAX_NAME_LEN 256
#define IPV4_LEN 4

enum node_status {
    NODE_ALIVE,
    NODE_SUSPECT,
    NODE_DEAD,
};

class node_state {
public:
    std::string name;
    uint32_t addr;
    uint32_t incarnation;
    node_status status;
    uint64_t timestamp;
};

class gossiper{
public:
    std::string name;
    uint32_t probe_index;

    std::vector<node_state *> nodes;
    std::map<uint32_t,node_state*> node_map;
    void probe_node(node_state *pstate);
    void probe_timeout(node_state *pstate);

public:
    void probe();
};
