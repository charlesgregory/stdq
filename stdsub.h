#define EOT '\x04'

/*! Address of ZeroMQ socket */
char *addr = "ipc:///tmp/stdpub";
//char *addr = "tcp://localhost:5556";

void jlog(char *format, ...);