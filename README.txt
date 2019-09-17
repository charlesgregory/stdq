stdq: Route stdin/stdout through ZeroMQ
=======================================


stdpub/stdsub
-------------

`cat <file> | ./stdpub`

elsewhere:

`./stdsub [-e] [socket]`

NOTES:
EOF is propagated over the channel as 0x04; stdsub recognizes this but does not close stdout/terminate and must be explicitly killed.
Passing option -e to stdsub will cause it to then pass the EOT marker (plus a newline, for line-oriented consumers) to stdout.
