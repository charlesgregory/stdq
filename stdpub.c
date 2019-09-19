#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>	// sleep
#include <zmq.h>

#include "stdpub.h"

int main(int argc, char **argv)
{
    // number read and published
    uint64_t nlines = 0;

    size_t buflen = 1048576;
    char *line = malloc(buflen);
    ssize_t linelen = 0;
   
    int major, minor, patch;
    zmq_version(&major, &minor, &patch);
    jlog("ZMQ version: %d.%d.%d", major, minor, patch);

    char *addr = "ipc:///tmp/stdpub";
    //char *addr = "tcp://*:5556";
    int opt;
    if (optind < argc)
        addr = argv[optind];
    jlog("Establishing ZMQ PUB socket at address: %s", addr);
    void *context = zmq_ctx_new();
    void *publisher = zmq_socket(context, ZMQ_PUB);
    int hwm = 0;  // high water mark on sender side default 1000, 0:no limit
    zmq_setsockopt(publisher, ZMQ_SNDHWM, &hwm, sizeof(hwm));
    int rc = zmq_bind(publisher, addr);
    assert(rc == 0);
    // TODO: error handling for rc < 0 http://api.zeromq.org/master:zmq-bind

    jlog("Waiting 2 seconds before streaming...");
    sleep(2);

    jlog("Streaming stdin to queue");
    while (1) {
        linelen = getline(&line, &buflen, stdin);
        if (linelen < 1)
            break;
        nlines++;
        
        zmq_send(publisher, line, linelen, 0);
    }
    // Issue EOT marker
    char eot[1] = {EOT};
    zmq_send(publisher, eot, 1, 0);

    jlog("%llu lines processed", nlines);

    jlog("ZMQ shutdown");
    zmq_close(publisher);
    zmq_ctx_destroy(context);

    return 0;
}
