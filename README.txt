stdq: Route stdin/stdout through ZeroMQ
=======================================


stdpub/stdsub
-------------

`cat <file> | ./stdpub`

elsewhere:

`./stdsub`

NOTES: EOF is not propagated over the channel; stdsub must be explicitly killed

