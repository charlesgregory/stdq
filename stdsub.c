#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>	// STDOUT_FILENO
#include <zmq.h>

#include "stdsub.h"

int main(int argc, char **argv)
{
    // number read and emitted
    uint64_t nlines = 0;

    size_t buflen = 1048576;
    char *line = malloc(buflen);
    ssize_t linelen = 0;

    int opt;
    int send_eot = 0;   // option: propagate EOT marker byte (but don't close stdout)
    while ((opt = getopt(argc, argv, "e")) != -1) {
        switch (opt) {
            case 'e': send_eot = 1; break;
            default:
                fprintf(stderr, "Usage: %s [-e] [socket]\tDefault socket: %s\n", argv[0], addr);
                exit(EXIT_FAILURE);
        }
    }
    // Now optind (declared extern int by <unistd.h>) is the index of the first non-option argument.
    // If it is >= argc, there were no non-option arguments.
    if (optind < argc)
        addr = argv[optind];
    
    int major, minor, patch;
    zmq_version(&major, &minor, &patch);
    jlog("ZMQ version: %d.%d.%d", major, minor, patch);   
    
    jlog("Establishing ZMQ SUB socket at address: %s", addr);
    void *context = zmq_ctx_new();
    void *subscriber = zmq_socket(context, ZMQ_SUB);
    int rc = zmq_connect(subscriber, addr);
    assert(rc == 0);
    // TODO: error handling for rc < 0 http://api.zeromq.org/master:zmq-bind

    // subscribe to all messages
    rc = zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "", 0);
    assert(rc == 0);

    jlog("Streaming queue to stdout");
    if (send_eot) jlog("Option: Propagate EOT marker");
    while (1) {
        int nbytes = zmq_recv(subscriber, line, buflen, 0);

        if ( nbytes > (buflen >> 1)  )
        {
            if ( nbytes <= buflen )
                jlog("Recieved message greater than 50%% of buffer size (%lu), expanding 2x", buflen);
            else if ( nbytes > buflen )
                jlog("Dropped part of message greater than buffer size (%lu); expanding 2x", buflen);
                // TODO actually drop the entire message, or rewrite line if format known
            else {
                jlog("Logic error");
                exit(EXIT_FAILURE);
            }

            buflen *= 2;
            line = realloc(line, buflen);
            if (line == NULL) {
                jlog("Memory reallocation for buffer size %lu failed", buflen);
                exit(1);
            }
        }

        if (nbytes == 1 && line[0] == EOT) {
            jlog("EOT");
            if (send_eot) write(STDOUT_FILENO, line, 1);
        }
        else {
            write(STDOUT_FILENO, line, nbytes); 
            nlines++;
        }
    }

    jlog("%llu lines processed", nlines);

    jlog("ZMQ shutdown");
    zmq_close(subscriber);
    zmq_ctx_destroy(context);

    return 0;
}
